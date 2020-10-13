// A simple shader
#version 410

layout (location = 0) in vec4 aPosition;
layout (location = 1) in vec4 aColor;

out vec4 vertexColor;

uniform mat4 ProjectionViewModel;

void main() {
	vertexColor = aColor;
	gl_Position = ProjectionViewModel * aPosition;
}