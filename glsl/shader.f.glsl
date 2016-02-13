uniform sampler2D texUnion;
uniform float texCount;
uniform float texCurScheme;
uniform float isBG;
uniform float plasmaSpeed;

varying vec2 f_texCoord;
flat varying float f_texIndex;
varying vec2 f_scrVertex;

#define PI 3.14159265359

// thanks to https://www.shadertoy.com/view/XtXGDS
vec4 plasma()
{
	float glTime = plasmaSpeed / 30.0;
	vec2 uv = vec2( f_scrVertex.x/2.0, f_scrVertex.y/2.0 );
    float a = abs( atan( uv.y / uv.x ) ) / PI;
    vec4 c = vec4( a, a, a, 1.0 );
    float off = 0; //0.5 * PI * 2.0;
    c.r += sin( glTime ) * 0.1;
    c.g += sin( glTime + off ) * 0.1;
    c.b += sin( glTime + off * 2.0 ) * 0.1;
	return c;
}

void main(void) 
{
	if ( isBG != 0.0 )
	{
		gl_FragColor = plasma();
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
