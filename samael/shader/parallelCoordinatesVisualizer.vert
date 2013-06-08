#version 430 core

layout(location = 1) in vec2 in_linePoint;
layout(location = 2) in vec3 in_lineColor;

out uint vs_out_color;

void main()
{
	vs_out_color = packUnorm4x8(vec4(in_lineColor, 1));
	gl_Position = vec4(in_linePoint.xy - vec2(0.999, 1.0),0,1);
}