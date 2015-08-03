#version 400
	
layout( location = 0 ) out vec4 FragColor;
varying vec4 Color;
void main()
{
	FragColor = Color;
}