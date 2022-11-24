#shader vertex
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
}