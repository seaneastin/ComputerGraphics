// A simple color shader
#version 410

in vec4 vertexColor;

out vec4 FragColor;

void main() {
	FragColor = vertexColor;
}