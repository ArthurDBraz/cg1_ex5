#version 150

in vec2 texCoords;

out vec4 pixel;

uniform sampler2D tex;

void main() {
	pixel = texture(tex, texCoords);
}