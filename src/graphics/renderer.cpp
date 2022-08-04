#include "renderer.hpp"
#include "logger.hpp"
#include "gtc/matrix_transform.hpp"
#include "globals.hpp"
#include "drawable.hpp"

namespace prim
{

	void glClearError()
	{
		while (glGetError() != GL_NO_ERROR)
			;
	}

	bool glLogCall(const char* function, const char* file, int line)
	{
		if (GLenum error = glGetError())
		{
			Logger::log("[OpenGL Error] (" + std::to_string(error) + "): " + function + " " + file + ":" + std::to_string(line), true);
			return false;
		}
		return true;
	}

	Renderer::Renderer()
	{
	}

	Renderer::~Renderer()
	{
		glfwTerminate();
		Logger::log("GLFW successfully terminated", true);
	}

	void Renderer::init(unsigned int windowWidth, unsigned int windowHeight, const char* windowName)
	{
		if (!glfwInit())
		{
			throw std::runtime_error("Failed to initialize glfw!");
		}

		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(windowWidth, windowHeight, windowName, nullptr, nullptr);
		if (window == nullptr)
		{
			throw std::runtime_error("Failed to create glfw window!");
		}

		this->windowWidth = windowWidth;
		this->windowHeight = windowHeight;

		windowRendererMap[window] = this;

		glfwMakeContextCurrent(window);
		glewExperimental = true;
		glViewport(0, 0, windowWidth, windowHeight);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		if (glewInit() != GLEW_OK)
		{
			throw std::runtime_error("Failed to initialize glew!");
		}

		glfwSetErrorCallback(error_callback);

		glfwSwapInterval(1);

		GL_CALL(glEnable(GL_BLEND));
		GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		GL_CALL(glEnable(GL_DEPTH_TEST));
		GL_CALL(glDepthFunc(GL_LEQUAL));

		Logger::log("GLFW and GLEW initialized successfully", true);
		Logger::log("OpenGL version: " + std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION))), true);
		Logger::log("GPU: " + std::string(reinterpret_cast<const char*>(glGetString(GL_RENDERER))), true);

		selectShader = new Shader("./res/shaders/select.shader");
	}

	void Renderer::drawLists()
	{
		prepareForDrawing();

		for (const Mesh* mesh : drawList)
		{
			mesh->va.bind();
			for (const MeshComposition& composition : mesh->compositions)
			{
				composition.ib.bind();
				composition.texture.bind();
				if (composition.shader.getId() != currentShaderId)
				{
					composition.shader.bind();
					currentShaderId = composition.shader.getId();
				}

				GL_CALL(glDrawElements(GL_TRIANGLES, composition.getCount(), GL_UNSIGNED_INT, nullptr));
			}
		}
	}

	void Renderer::prepareForDrawing()
	{
		if (preparedForDrawing) return;

		if (currentCamera)
		{
			setViewMat(currentCamera->calculateViewMatrix());
			setProjectMat(currentCamera->calculateProjectMatrix());
		}
		else
		{
			setViewMat(glm::mat4(1.0f));
			setProjectMat(glm::ortho(0.0f, static_cast<float>(windowWidth), 0.0f, static_cast<float>(windowHeight)));
		}

		preparedForDrawing = true;
	}

	void Renderer::drawSelectedNodeFraming(Drawable* node)
	{
		GL_CALL(glEnable(GL_STENCIL_TEST));

		GL_CALL(glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));
		GL_CALL(glStencilFunc(GL_ALWAYS, 1, 0xFF));
		GL_CALL(glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE));
		node->draw(*this, selectShader);

		GL_CALL(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));
		GL_CALL(glStencilFunc(GL_NOTEQUAL, 1, 0xFF));
		GL_CALL(glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP));
		node->scale(1.05f);
		node->draw(*this, selectShader);
		node->scale(1.0f/1.05f);
		
		GL_CALL(glDisable(GL_STENCIL_TEST));
	}

	void Renderer::drawMesh(const Mesh& mesh)
	{
		prepareForDrawing();

		mesh.va.bind();
		const glm::mat4 mvp = projectMat * viewMat * modelMat;
		for (const MeshComposition& composition : mesh.compositions)
		{
			composition.ib.bind();
			composition.texture.bind();
			if (composition.shader.getId() != currentShaderId)
			{
				composition.shader.bind();
				currentShaderId = composition.shader.getId();
			}

			composition.shader.setUniformMat4f("u_mvp", mvp);

			GL_CALL(glDrawElements(GL_TRIANGLES, composition.getCount(), GL_UNSIGNED_INT, nullptr));
		}
	}

	void Renderer::drawMesh(const Mesh& mesh, const Shader* shader)
	{
		prepareForDrawing();

		mesh.va.bind();
		const glm::mat4 mvp = projectMat * viewMat * modelMat;
		shader->bind();
		shader->setUniformMat4f("u_mvp", mvp);
		for (const MeshComposition& composition : mesh.compositions)
		{
			composition.ib.bind();
			composition.texture.bind();

			GL_CALL(glDrawElements(GL_TRIANGLES, composition.getCount(), GL_UNSIGNED_INT, nullptr));
		}
	}

	void Renderer::addMesh(Mesh* mesh)
	{
		drawList.push_back(mesh);
	}

	void Renderer::clear()
	{
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
	}

	void Renderer::framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		Renderer* renderer = windowRendererMap[window];
		renderer->windowWidth = width;
		renderer->windowHeight = height;
		glViewport(0, 0, width, height);
	}

	void Renderer::error_callback(int error, const char* description)
	{
		Logger::log("GLFW error: " + std::to_string(error), true);
		Logger::log(description);
		// std::cerr << "GLFW error: " << error << std::endl;
		// std::cerr << description;
	}

	bool Renderer::windowShouldClose()
	{
		return glfwWindowShouldClose(window);
	}

	void Renderer::swapBuffers()
	{
		glfwSwapBuffers(window);
		preparedForDrawing = false;
	}

	void Renderer::pollEvents()
	{
		glfwPollEvents();
	}

	const std::vector<Mesh*>& Renderer::getDrawList() const
	{
		return drawList;
	}

}
