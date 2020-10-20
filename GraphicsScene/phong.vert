//Classic Phong vertex shader
#version 410

layout (location = 0) in vec4 aPosition;
layout (location = 1) in vec4 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 4) in vec4 aColor;

out vec3 vNormal;
out vec4 vColor;
out vec2 vTexCoord;

uniform mat4 ProjectionViewModel;
uniform mat3 NormalMatrix;

void main() {
	vNormal = NormalMatrix * aNormal.xyz;
	vColor = aColor;
	vTexCoord = aTexCoord;
	gl_Position = ProjectionViewModel * aPosition;
}