#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D image;
uniform vec2 uvMin;
uniform vec2 uvMax;

void main()
{
    vec2 uv = mix(uvMin, uvMax, TexCoords);
    FragColor = texture(image, uv);
}
