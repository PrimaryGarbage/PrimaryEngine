#include "renderer.hpp"
#include "logger.hpp"

namespace prim
{

	void glClearError()
	{
		while (glGetError() != GL_NO_ERROR);
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

	void Renderer::drawLists() const
	{
		for(Model* model : modelDrawList)
		{
			for (const Mesh& mesh : model->meshes)
			{
				mesh.va.bind();
				for (const MeshComposition& composition : mesh.compositions)
				{
					composition.bind();
					GL_CALL(glDrawElements(GL_TRIANGLES, composition.getCount(), GL_UNSIGNED_INT, nullptr));
				}
			}
		}
	}

	void Renderer::drawModel(const Model& model)
	{
		for (const Mesh& mesh : model.meshes)
		{
			mesh.va.bind();
			for (const MeshComposition& composition : mesh.compositions)
			{
				composition.bind();
				GL_CALL(glDrawElements(GL_TRIANGLES, composition.getCount(), GL_UNSIGNED_INT, nullptr));
			}
		}
	}

	void Renderer::addModel(Model* model)
	{
		modelDrawList.push_back(model);
	}

	void Renderer::clear()
	{
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
	}

}
