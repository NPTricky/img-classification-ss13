#version 430 core

layout(lines) in;
layout(line_strip, max_vertices = 5) out;

void main()
{
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();

	gl_Position = vec4(gl_in[1].gl_Position.x, gl_in[0].gl_Position.yzw);
	EmitVertex();

	gl_Position = gl_in[1].gl_Position;
	EmitVertex();

	gl_Position = vec4(gl_in[0].gl_Position.x, gl_in[1].gl_Position.yzw);
	EmitVertex();

	gl_Position = gl_in[0].gl_Position;
	EmitVertex();

	EndPrimitive();
}