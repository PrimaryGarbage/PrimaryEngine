#shader vertex
#version 330 core

layout(location = 0)in vec4 position;
layout(location = 1)in vec2 texCoord;

out vec2 uv;

void main()
{
	uv = texCoord;
	gl_Position = position;
}

#shader fragment
#version 330 core

layout(location = 0)out vec4 color;

in vec2 uv;

uniform vec4 u_color;
uniform sampler2D u_texture;

void main()
{
	vec4 texColor = texture(u_texture, uv);
	color = texColor;
}