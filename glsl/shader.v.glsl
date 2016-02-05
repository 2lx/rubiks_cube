attribute vec3 cVertex;
attribute vec2 texCoord;
attribute float texIndex;

uniform mat4 mvp;

varying vec2 f_texCoord;
flat varying float f_texIndex;

void main(void) 
{
	gl_Position = mvp * vec4(cVertex, 1.0);
	f_texCoord = texCoord;
	f_texIndex = texIndex;
}
