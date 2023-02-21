#include "vertex_buffer.hpp"
#include "macros.hpp"
#include "glfw_extensions.hpp"
#include "macros.hpp"

namespace prim
{

VertexBuffer::VertexBuffer(const void* data, unsigned int size, VertexBufferLayout layout) : layout(layout)
{
	GL_CALL(glGenVertexArrays(1, &gl_id_vao));
	GL_CALL(glBindVertexArray(gl_id_vao));

	GL_CALL(glGenBuffers(1, &gl_id_vbo));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, gl_id_vbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	layout.bind();
}

VertexBuffer::VertexBuffer(VertexBuffer&& other)
	: gl_id_vbo(other.gl_id_vbo), gl_id_vao(other.gl_id_vao), layout(other.layout)
{
	other.gl_id_vbo = 0;
	other.gl_id_vao = 0;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other)
{
	unload();

	gl_id_vbo = other.gl_id_vbo;
	gl_id_vao = other.gl_id_vao;
	layout = other.layout;

	other.gl_id_vbo = 0;
	other.gl_id_vao = 0;

	return *this;
}

VertexBuffer::~VertexBuffer()
{
	unload();
}

void VertexBuffer::bind() const
{
	if (gl_id_vao > 0)
		GL_CALL(glBindVertexArray(gl_id_vao));
}

void VertexBuffer::unbind() const
{
	GL_CALL(glBindVertexArray(0u));
}

void VertexBuffer::unload()
{
	ASSERT_GLFW_NOT_TERMINATED
	if(gl_id_vao > 0)
	{
		unbind();
		GL_CALL(glDeleteVertexArrays(1, &gl_id_vao));
		GL_CALL(glDeleteBuffers(1, &gl_id_vbo));
		gl_id_vbo = 0;
		gl_id_vao = 0;
	}
}

}


