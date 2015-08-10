#version 330
uniform samplerCube uTexture;

smooth in vec3 eyeDirection;

out vec4 fragmentColor;

void main() {

//    fragmentColor = texture(uTexture, eyeDirection);
fragmentColor=vec4(0.5,0.5,0.5,1);
	}