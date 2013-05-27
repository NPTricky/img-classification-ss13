#version 430 core

layout(location = 0) in vec2 in_keyPoint;

out vec2 vs_out_keyPoint;

void main()
{
	gl_Position = vec4(vs_out_keyPoint.xy,0,1);
}