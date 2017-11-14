#version 150

in vec2 pos;
uniform float rot = 0;
uniform float scale = 1;
uniform vec2 position;

uniform mat4 proj;

void main()
{
	gl_Position = vec4( pos, 1.0, 1.0 );
	float cosRot = cos( rot );
	float sinRot = sin( rot );
	
	gl_Position.x = scale * ( pos.x * cosRot - pos.y * sinRot );
	gl_Position.y = scale * ( pos.x * sinRot + pos.y * cosRot );
	gl_Position = proj * ( gl_Position ) + vec4( position, 0, 0 );
}