#version 150

in vec2 texCoords;
in vec4 vertexPosition;
in vec3 vertexNormal;

out vec4 pixel;

uniform bool showTexture;
uniform bool lighting;
uniform sampler2D tex;

vec4 blinnPhongReflection(vec4 position, vec3 normal);

void main(void) {
	vec4 color = vec4(.5f, .5f, .5f, 1.0f);

	if (showTexture) {
		color = texture2D(tex, texCoords);
	}

	if (lighting) {
		color *= blinnPhongReflection(vertexPosition, vertexNormal);
	}
	pixel = color;
	
}