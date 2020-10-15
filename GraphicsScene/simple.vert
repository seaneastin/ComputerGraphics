// A simple shader
#version 410

layout (location = 0) in vec4 aPosition;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 vColor;
out vec2 vTexCoord;

uniform mat4 ProjectionViewModel;

void main() {
	vColor = aColor;
	vTexCoord = aTexCoord;
	gl_Position = ProjectionViewModel * aPosition;
}