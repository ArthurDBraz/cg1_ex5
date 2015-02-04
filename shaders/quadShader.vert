#version 150

in vec4 position;

out vec2 texCoords;
out vec4 vertexPosition;
out vec3 vertexNormal;

uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat4 normalMatrix;

void main() {
	vertexPosition = modelViewMatrix * position;
	// Normal vector: the mesh is a plane quad, so there is one simple normal on z direction
	vertexNormal = normalize((normalMatrix * vec4(0.f, 0.f, 1.f, 1.f)).xyz);
	// transform from (-1,1) to (0,1) for the quad
	texCoords = (position.xy * 0.5f) + (0.5, 0.5);
	gl_Position = modelViewProjectionMatrix * position;
}