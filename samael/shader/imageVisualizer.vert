#version 430 core

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec2 in_texCoord;

out vec2 vs_out_texCoord;

void main()
{
	gl_Position = vec4(in_pos, 1);
	vs_out_texCoord = in_texCoord;
}