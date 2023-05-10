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

static const char* frameShaderData = 
R"rawstring(#shader vertex
#version 330 core

layout(location = 0)in vec4 position;
layout(location = 1)in vec2 texCoord;
layout(location = 2)in vec4 vertexNormal;

out vec2 uv;

uniform mat4 u_mvp;
uniform float u_frameWidth = 1.2;

void main()
{
	uv = texCoord;

	vec4 newPosition = position;
	newPosition.xyz += vertexNormal.xyz * u_frameWidth;

	gl_Position = u_mvp * newPosition;
}

#shader fragment
#version 330 core

layout(location = 0)out vec4 color;

in vec2 uv;

uniform vec4 u_color = vec4(1.0f, 1.0f, 0.0f, 0.8f);

void main()
{
	color = u_color;
})rawstring";

static const char* defaultTextShaderData = 
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
uniform int u_sampleTexture = 0;
uniform sampler2D u_texture;
uniform float u_borderRadius = 0.0;
uniform vec2 u_resolution = vec2(1.0, 1.0);

float resRatio = u_resolution.y / u_resolution.x;

float isOutsideOfCircle(float px, float py, float cx, float cy, float rad)
{
    float pxAdj = px - cx;
    float pyAdj = py - cy;
    return step(rad * rad, pxAdj * pxAdj + pyAdj * pyAdj);
}

float notInLeftUpperCorner(vec2 uv, float borderRadius)
{
    return 1.0 - (1.0 - step(borderRadius, uv.x)) * (step(resRatio - borderRadius, uv.y)) * (isOutsideOfCircle(uv.x, uv.y, borderRadius, resRatio - borderRadius, borderRadius));
}

float notInLeftLowerCorner(vec2 uv, float borderRadius)
{
    return 1.0 - (1.0 - step(borderRadius, uv.x)) * (1.0 - step(borderRadius, uv.y)) * (isOutsideOfCircle(uv.x, uv.y, borderRadius, borderRadius, borderRadius));
}

float notInRightUpperCorner(vec2 uv, float borderRadius)
{
   return 1.0 - (step(1.0 - borderRadius, uv.x)) * (step(resRatio - borderRadius, uv.y)) * (isOutsideOfCircle(uv.x, uv.y, 1.0 - borderRadius, resRatio - borderRadius, borderRadius));
}

float notInRightLowerCorner(vec2 uv, float borderRadius)
{
    return 1.0 - (step(1.0 - borderRadius, uv.x)) * (1.0 - step(borderRadius, uv.y)) * (isOutsideOfCircle(uv.x, uv.y, 1.0 - borderRadius, borderRadius, borderRadius));
}

void main()
{
	float bRad = u_borderRadius / u_resolution.x;
	vec2 uvScaled = vec2(uv.x, uv.y * u_resolution.y / u_resolution.x);
	float visible = notInLeftUpperCorner(uvScaled, bRad) * notInRightUpperCorner(uvScaled, bRad) * notInLeftLowerCorner(uvScaled, bRad) * notInRightLowerCorner(uvScaled, bRad);
	vec4 texColor = texture(u_texture, uv);
	color = vec4(u_color.xyz, visible * u_color.a);
	if(u_sampleTexture == 1)
	{
		color = color * texColor;
	}
})rawstring";

#endif // __DEFAULT_SHADER_DATA_HPP__