#version 150

in vec4 position;
in vec3 normal;

out vec2 texCoords;
out vec4 vertexPosition;
out vec3 vertexNormal;

uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat4 normalMatrix;
uniform mat4 mirrorMatrix;

void main() {
	vertexPosition = modelViewMatrix * position;
	vertexNormal = normalize((normalMatrix * vec4(normal, 1.f)).xyz);
	
	// vector that comes from the camera and point to the origin
	vec3 eyeVector = normalize( vec3( vertexPosition.xyz ) );
	
	// calculate reflection vector
	vec3 reflectionVector = reflect(eyeVector, vertexNormal);
	
	// calculate de the modulo
	float modulo = 2. * sqrt( 
		pow( reflectionVector.x, 2. ) + 
		pow( reflectionVector.y, 2. ) + 
		pow( reflectionVector.z + 1., 2. ) );
	
	// transform to uv coordinates
	texCoords = reflectionVector.xy / modulo + .5;
	gl_Position = modelViewProjectionMatrix * position;
}