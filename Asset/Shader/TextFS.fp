#version 400
in vec2 outVaryingTexCoord;
out vec4 OutColor;

uniform sampler2D tex2D;

void main ()
{
	OutColor = texture ( tex2D, outVaryingTexCoord.st );
}