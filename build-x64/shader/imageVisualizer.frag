#version 430 core

layout(binding = 0) uniform sampler2D imageTexture;

in vec2 vs_out_texCoord;

out vec4 out_Color;

void main()
{
	out_Color = texture(imageTexture, vs_out_texCoord);
}