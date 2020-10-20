//Classic Phong fragment shader
#version 410

in vec3 vNormal;
in vec4 vColor;
in vec2 vTexCoord;

uniform vec3 Ia;
uniform vec3 Id;
uniform vec3 Is;
uniform vec3 LightDirection;

out vec4 FragColor;

void main() {
	//Ensure normal and light direction are normalized
	vec3 N = normalize(vNormal);
	vec3 L = normalize(LightDirection);

	//Calculate lambert term (negate light direction)
	float lambertTerm = max(0, min(1, dot(N, -L)));

	//Output color
	vec4 ambient = vColor * vec4(Ia, 1);
	vec4 diffuse = vColor * vec4(Id, 1) * vec4(lambertTerm, lambertTerm, lambertTerm, 1);
	FragColor = ambient + diffuse;
}