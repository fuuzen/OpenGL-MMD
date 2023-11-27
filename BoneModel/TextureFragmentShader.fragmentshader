#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec2 Tex;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler1;
uniform sampler2D myTextureSampler2;
uniform sampler2D myTextureSampler3;
uniform sampler2D myTextureSampler4;
uniform sampler2D myTextureSampler5;
uniform sampler2D myTextureSampler6;
uniform sampler2D myTextureSampler7;
uniform sampler2D myTextureSampler8;
uniform sampler2D myTextureSampler9;

void main(){

	// Output color = color of the texture at the specified UV
	if (Tex.x > 0.89)
	{
		color = texture( myTextureSampler9, UV ).rgb;
	}
	else if (Tex.x > 0.79)
	{
		color = texture( myTextureSampler8, UV ).rgb;
	}
	else if (Tex.x > 0.69)
	{
		color = texture( myTextureSampler7, UV ).rgb;
	}
	else if (Tex.x > 0.59)
	{
		color = texture( myTextureSampler6, UV ).rgb;
	}
	else if (Tex.x > 0.49)
	{
		color = texture( myTextureSampler5, UV ).rgb;
	}
	else if (Tex.x > 0.39)
	{
		color = texture( myTextureSampler4, UV ).rgb;
	}
	else if (Tex.x > 0.29)
	{
		color = texture( myTextureSampler3, UV ).rgb;
	}
	else if (Tex.x > 0.19)
	{
		color = texture( myTextureSampler2, UV ).rgb;
	}
	else if (Tex.x > 0.09)
	{
		color = texture( myTextureSampler1, UV ).rgb;
	}
	else
	{
		color = vec3(0.0, 0.0, 0.0);
	}
}