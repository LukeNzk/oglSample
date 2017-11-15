#version 150

out vec4 color;
in vec2 uvs;
uniform sampler2D tex0;

void main()
{
  color = texture( tex0, uvs.st ).rgba;
}