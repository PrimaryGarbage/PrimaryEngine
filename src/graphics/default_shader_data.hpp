#ifndef __DEFAULT_SHADER_DATA_HPP__
#define __DEFAULT_SHADER_DATA_HPP__

static const char* defaultShaderData = 
R"rawstring(#shader vertex
#version 330 core

layout(location = 0)in vec4 position;
layout(location = 1)in vec2 texCoord;
layout(location = 2)in vec4 vertexNormal;

out vec2 uv;

uniform mat4 u_mvp;
uniform float u_time;

void main()
{
	uv = texCoord;

	gl_Position = u_mvp * position;
}

#shader fragment
#version 330 core

layout(location = 0)out vec4 color;

in vec2 uv;

uniform vec4 u_color;
uniform sampler2D u_texture;
uniform float u_time;

void main()
{
	vec4 texColor = texture(u_texture, uv);
	color = texColor;
})rawstring";

static const char* selectShaderData = 
R"rawstring(#shader vertex
#version 330 core

layout(location = 0)in vec4 position;
layout(location = 1)in vec2 texCoord;
layout(location = 2)in vec4 vertexNormal;

out vec2 uv;

uniform mat4 u_mvp;
uniform float u_time;
const float borderWidth = 5.0f;

void main()
{
	uv = texCoord;

	vec4 newPosition = position;
	newPosition.xyz += vertexNormal.xyz * borderWidth;

	gl_Position = u_mvp * newPosition;
}

#shader fragment
#version 330 core

layout(location = 0)out vec4 color;

in vec2 uv;

uniform vec4 u_color;
uniform sampler2D u_texture;
uniform float u_time;
const vec4 select_color = vec4(1.0f, 1.0f, 0.0f, 0.8f);

void main()
{
	color = select_color;
})rawstring";

static const char* textDefaultShaderData = 
R"rawstring(#shader vertex
#version 330 core

layout(location = 0)in vec4 position;
layout(location = 1)in vec2 texCoord;
layout(location = 2)in vec4 vertexNormal;

out vec2 uv;

uniform mat4 u_mvp;
uniform float u_time;

void main()
{
	uv = texCoord;

	gl_Position = u_mvp * position;
}

#shader fragment
#version 330 core

layout(location = 0)out vec4 color;

in vec2 uv;

uniform vec4 u_color;
uniform sampler2D u_texture;
uniform float u_time;

void main()
{
	vec4 texColor = texture(u_texture, uv);
	color = texColor;
})rawstring";

#endif // __DEFAULT_SHADER_DATA_HPP__