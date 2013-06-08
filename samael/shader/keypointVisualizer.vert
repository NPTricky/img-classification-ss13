#version 430 core

layout(location = 0) in vec2 in_keyPoint;

void main()
{
	gl_Position = vec4(in_keyPoint.xy,0,1);
}