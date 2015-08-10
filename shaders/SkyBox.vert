#version 400

uniform mat4 uProjection;
uniform mat4 uView; //view

in vec4 aPosition;

smooth out vec3 eyeDirection;

void main() {
    mat4 inverseProjection = inverse(uProjection);
    mat3 inverseModelview = transpose(mat3(uView));
    vec3 unprojected = (inverseProjection * aPosition).xyz;
    eyeDirection = inverseModelview * unprojected;

    gl_Position = aPosition;
} 