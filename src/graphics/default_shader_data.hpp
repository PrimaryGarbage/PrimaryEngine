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

uniform vec4 u_color = vec4(1.0, 1.0, 1.0, 1.0);
uniform sampler2D u_texture;
uniform float u_time;

void main()
{
	vec4 texColor = texture(u_texture, uv);
	color = texColor * u_color;
})rawstring";

static const char* defaultPlainColorShaderData = 
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
uniform float u_time;
uniform float u_borderRadius;

void main()
{
	color = u_color;
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

uniform vec4 u_color = vec4(1.0, 1.0, 1.0, 1.0);
uniform sampler2D u_texture;
uniform float u_time;

void main()
{
	vec4 texColor = vec4(1.0, 1.0, 1.0, texture(u_texture, uv).r);
	color = texColor * u_color;
})rawstring";

static const char* defaultControlBackgroundShaderData = 
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
uniform float u_time;
uniform float u_borderRadius = 0.0;

float isOutsideOfCircle(float px, float py, float cx, float cy, float rad)
{
    float pxAdj = px - cx;
    float pyAdj = py - cy;
    return step(rad * rad, pxAdj * pxAdj + pyAdj * pyAdj);
}

float notInLeftUpperCorner()
{
    return 1.0 - (1.0 - step(u_borderRadius, uv.x)) * (step(1.0 - u_borderRadius, uv.y)) * (isOutsideOfCircle(uv.x, uv.y, u_borderRadius, 1.0 - u_borderRadius, u_borderRadius));
}

float notInLeftLowerCorner()
{
    return 1.0 - (1.0 - step(u_borderRadius, uv.x)) * (1.0 - step(u_borderRadius, uv.y)) * (isOutsideOfCircle(uv.x, uv.y, u_borderRadius, u_borderRadius, u_borderRadius));
}

float notInRightUpperCorner()
{
   return 1.0 - (step(1.0 - u_borderRadius, uv.x)) * (step(1.0 - u_borderRadius, uv.y)) * (isOutsideOfCircle(uv.x, uv.y, 1.0 - u_borderRadius, 1.0 - u_borderRadius, u_borderRadius));
}

float notInRightLowerCorner()
{
    return 1.0 - (step(1.0 - u_borderRadius, uv.x)) * (1.0 - step(u_borderRadius, uv.y)) * (isOutsideOfCircle(uv.x, uv.y, 1.0 - u_borderRadius, u_borderRadius, u_borderRadius));
}

void main()
{
	float visible = notInLeftUpperCorner() * notInRightUpperCorner() * notInLeftLowerCorner() * notInRightLowerCorner();
	color = vec4(u_color.xyz, visible);
})rawstring";

#endif // __DEFAULT_SHADER_DATA_HPP__