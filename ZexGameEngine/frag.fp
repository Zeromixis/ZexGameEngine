#version 400
//in vec4 OutVertexColor;
uniform sampler1D tex1D;
in vec2 vVaryingTexCoords;
out vec4 OutColor;


void main ()
{
	OutColor = texture ( tex1D, vVaryingTexCoords.s );
}