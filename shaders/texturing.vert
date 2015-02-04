#version 150

in vec4 position;
in vec2 texCoord;
in vec3 normal;

out vec2 texCoords;
out vec4 vertexPosition;
out vec3 vertexNormal;

uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat4 normalMatrix;

void main() {
	vertexPosition = modelViewMatrix * position;
	
	// Use the normals calculated and sphere coordinates pre-calculated
	vertexNormal = normalize((normalMatrix * vec4(normal, 1.f)).xyz);
	texCoords = texCoord;
	
	gl_Position = modelViewProjectionMatrix * position;
}