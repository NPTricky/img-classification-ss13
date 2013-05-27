#version 430 core

layout(points) in;
layout(line_strip, max_vertices = 4) out;

void main()
{
	vec2 crossSize = vec2(0.02, 0.02);

	gl_Position = vec4(gl_in[0].gl_Position.xy + crossSize, 0, 1);
	EmitVertex();
	gl_Position = vec4(gl_in[0].gl_Position.xy - crossSize, 0, 1);
	EmitVertex();
	EndPrimitive();

	gl_Position = vec4(gl_in[0].gl_Position.x + crossSize.x, gl_in[0].gl_Position.y - crossSize.y, 0, 1);
	EmitVertex();
	gl_Position = vec4(gl_in[0].gl_Position.x - crossSize.x, gl_in[0].gl_Position.y + crossSize.y, 0, 1);
	EmitVertex();
	EndPrimitive();
}