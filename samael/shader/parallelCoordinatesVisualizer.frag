#version 430 core

flat in uint gs_out_color;

out vec4 out_Color;

void main()
{
	out_Color = unpackUnorm4x8(gs_out_color);
}