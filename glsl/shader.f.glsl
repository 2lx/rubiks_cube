uniform sampler2D texUnion;

varying vec2 f_texCoord;
flat varying float f_texIndex;

void main(void) 
{
	if ( f_texIndex >= 0 && f_texIndex <= 5 )
	{
		vec2 flipped_texcoord = vec2( (f_texIndex + f_texCoord.x ) / 6.0, 1.0 - f_texCoord.y);
		gl_FragColor = texture2D(texUnion, flipped_texcoord);
	}
	else gl_FragColor = vec4(0, 0, 0, 1);
}
