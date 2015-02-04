#version 150

in vec4 position;

out vec2 texCoords;

void main() {
	// transform from (-1,1) to (0,1) for the quad
	texCoords = (position.xy * 0.5f) + (0.5, 0.5);
	gl_Position = position;
}