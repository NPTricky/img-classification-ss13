#version 430 core

layout(lines) in;
layout(line_strip, max_vertices = 2) out;

in uint vs_out_color[];

flat out uint gs_out_color;

void main()
{
	if(gl_in[1].gl_Position.x > -0.999f)
	{
		gl_Position = vec4(gl_in[0].gl_Position.xy, 0, 1);
		gs_out_color = vs_out_color[0];
		EmitVertex();

		gl_Position = vec4(gl_in[1].gl_Position.xy, 0, 1);
		gs_out_color = vs_out_color[1];
		EmitVertex();
		EndPrimitive();
	}
}