#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D camtexture1;

void main()
{
	vec4 container = texture(camtexture1, TexCoord);
	FragColor = container;
}