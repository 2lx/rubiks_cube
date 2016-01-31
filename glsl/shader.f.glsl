uniform sampler2D texBlue;
uniform sampler2D texWhite;
uniform sampler2D texGreen;
uniform sampler2D texYellow;
uniform sampler2D texRed;
uniform sampler2D texOrange;

varying vec2 f_texCoord;
flat varying float f_texIndex;

void main(void) 
{
	vec2 flipped_texcoord = vec2(f_texCoord.x, 1.0 - f_texCoord.y);
	switch ( int( f_texIndex ) )
	{
	case 0: 
		gl_FragColor = texture2D(texBlue, flipped_texcoord);
		break;
	case 1: 
		gl_FragColor = texture2D(texWhite, flipped_texcoord);
		break;
	case 2: 
		gl_FragColor = texture2D(texGreen, flipped_texcoord);
		break;
	case 3: 
		gl_FragColor = texture2D(texYellow, flipped_texcoord);
		break;
	case 4: 
		gl_FragColor = texture2D(texRed, flipped_texcoord);
		break;
	case 5: 
		gl_FragColor = texture2D(texOrange, flipped_texcoord);
		break;
	default:
		gl_FragColor = vec4(0, 0, 0, 1);
	};
}
