#version 330
in vec2 UV;
out vec3 OutColor;

uniform sampler2D BmpSampler;   // Texture 0

void main ()
{
	OutColor = texture ( BmpSampler, UV ).rgb;
}