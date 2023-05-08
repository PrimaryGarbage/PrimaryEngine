#include "renderer.hpp"
#include "logger.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "globals.hpp"
#include "nodes/drawable.hpp"
#include "nodes/camera_base.hpp"
#include "glfw_extensions.hpp"
#include "primitives.hpp"


namespace prim
{
	Renderer::Renderer(uint32_t windowWidth, uint32_t windowHeight, const std::string& windowName)
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

		window = glfwCreateWindow(windowWidth, windowHeight, windowName.c_str(), nullptr, nullptr);
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

		Logger::inst().logInfo("GLFW and GLEW initialized successfully", true);
		Logger::inst().logInfo("OpenGL version: " + std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION))), true);
		Logger::inst().logInfo("GPU: " + std::string(reinterpret_cast<const char*>(glGetString(GL_RENDERER))), true);
	}

	Renderer::~Renderer()
	{
		Shader::terminate();
		Texture::terminate();
		glfwTerminate();
		Logger::inst().logInfo("GLFW successfully terminated", true);
	}

	void Renderer::drawLists()
	{
		prepareForDrawing();

		for (const Mesh* mesh : drawList)
		{
			mesh->vb->bind();
			for (const MeshComposition& composition : mesh->compositions)
			{
				composition.ib->bind();
				composition.texture->bind();
				if (composition.shader->getId() != currentShaderId)
				{
					composition.shader->bind();
					currentShaderId = composition.shader->getId();
				}

				GL_CALL(glDrawElements(GL_TRIANGLES, composition.getCount(), GL_UNSIGNED_INT, nullptr));
			}
		}
	}

	void Renderer::prepareForDrawing()
	{
		if (currentCamera)
		{
			if(!viewMatIsRelevant) setViewMat(currentCamera->calculateViewMatrix());
			if(!projectMatIsRelevant) setProjectMat(currentCamera->calculateProjectMatrix());
		}
		else
		{
			setViewMat(glm::mat4(1.0f));
			setProjectMat(glm::ortho(0.0f, static_cast<float>(windowWidth), 0.0f, static_cast<float>(windowHeight)));
		}
	}

	void Renderer::drawNodeFrame(Drawable* node, glm::vec4 color, float frameScale)
	{
		GL_CALL(glEnable(GL_STENCIL_TEST));

		GL_CALL(glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));
		GL_CALL(glStencilFunc(GL_ALWAYS, 1, 0xFF));
		GL_CALL(glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE));
		node->draw(*this);

		GL_CALL(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));
		GL_CALL(glStencilFunc(GL_NOTEQUAL, 1, 0xFF));
		GL_CALL(glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP));
		Shader* frameShader = Shader::getDefaultShader(DefaultShader::frame);
		frameShader->setUniform1f("u_frameWidth", frameScale);
		frameShader->setUniform4f("u_color", color);
		node->drawWithShader(*this, Shader::getDefaultShader(DefaultShader::frame));
		
		GL_CALL(glDisable(GL_STENCIL_TEST));
	}
	
	// NOT WORKING
	void Renderer::drawRectangle(glm::vec2 position, glm::vec2 size, glm::vec4 color)
	{
		glm::vec2 windowSize = getWindowSize();
		glm::vec2 viewportPos = position / windowSize;
		glm::vec2 viewportSize = size / windowSize;
		glColor4f(color.r, color.g, color.b, color.a);
		glRectf(viewportPos.x, viewportPos.y + viewportSize.y, viewportPos.x + viewportSize.x, viewportPos.y);
	}

	void Renderer::drawMesh(const Mesh& mesh)
	{
		prepareForDrawing();

		mesh.vb->bind();
		const glm::mat4 mvp = projectMat * viewMat * modelMat;
		for (const MeshComposition& composition : mesh.compositions)
		{
			composition.ib->bind();
			composition.texture->bind();
			if (composition.shader->getId() != currentShaderId)
			{
				composition.shader->bind();
				currentShaderId = composition.shader->getId();
			}

			composition.shader->setUniformMat4f("u_mvp", mvp);

			GL_CALL(glDrawElements(GL_TRIANGLES, composition.getCount(), GL_UNSIGNED_INT, nullptr));
		}
	}

	void Renderer::drawMesh(const Mesh& mesh, const Shader* shader)
	{
		prepareForDrawing();

		mesh.vb->bind();
		const glm::mat4 mvp = projectMat * viewMat * modelMat;
		shader->bind();
		shader->setUniformMat4f("u_mvp", mvp);
		for (const MeshComposition& composition : mesh.compositions)
		{
			composition.ib->bind();
			composition.texture->bind();

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
		Logger::inst().logError("GLFW error: " + std::to_string(error), true);
		Logger::inst().logError(description);
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

		viewMatIsRelevant = false;
		projectMatIsRelevant = false;
	}

	void Renderer::pollEvents()
	{
		glfwPollEvents();
	}

	const std::vector<Mesh*>& Renderer::getDrawList() const
	{
		return drawList;
	}
	
	void Renderer::setProjectMat(glm::mat4 proj) 
	{
		projectMat = proj;
		projectMatIsRelevant = true;
	}
	
	void Renderer::setViewMat(glm::mat4 view) 
	{
		viewMat = view;
		viewMatIsRelevant = true;
	}
	
	void Renderer::setModelMat(glm::mat4 model) 
	{
		modelMat = model;
	}

}
