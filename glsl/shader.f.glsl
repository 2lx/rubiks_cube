uniform sampler2D texUnion;
uniform float texCount;
uniform float texCurScheme;
uniform float isBG;

varying vec2 f_texCoord;
flat varying float f_texIndex;
varying vec2 f_scrVertex;

void main(void) 
{
	if ( isBG != 0.0 )
	{
		float clrR = 0.2843 + ( f_scrVertex.y + 1.0 ) * 0.1902;
		gl_FragColor = vec4( clrR, clrR, clrR, 1.0 );
	}
	else
	{
		if ( f_texIndex >= 0 && f_texIndex <= 5 )
		{
			vec2 flipped_texcoord = vec2( ( f_texIndex + f_texCoord.x ) / 6.0, 
				( 1.0 - f_texCoord.y - texCurScheme ) / texCount );
			gl_FragColor = texture2D(texUnion, flipped_texcoord);
		}
		else gl_FragColor = vec4(189.0f/255.0f, 195.0f/255.0f, 199.0f/255.0f, 1);
	}
}
