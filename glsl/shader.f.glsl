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

// thanks to https://www.shadertoy.com/view/ldBGRR
vec4 plasma2()
{
	float fade = plasmaSpeed / 50.0;
	vec2 p = vec2( -1.0 + f_scrVertex.x, -1.0 + f_scrVertex.y );
	
	// main code, *original shader by: 'Plasma' by Viktor Korsun (2011)
	float x = p.x;
	float y = p.y;
	float mov0 = x + y + cos( sin( fade ) * 2.0 ) * 100. + sin( x / 100. ) * 1000.;
	float mov1 = y / 0.9 +  fade;
	float mov2 = x / 0.2;
	float c1 = abs( sin( mov1 + fade ) / 2. + mov2 / 2. - mov1 - mov2 + fade );
	float c2 = abs( sin( c1 + sin( mov0 / 1000. + fade ) + sin( y / 40. + fade ) + sin( ( x + y ) / 100. ) * 3. ) );
	float c3 = abs( sin( c2 + cos( mov1 + mov2 + c2 ) + cos( mov2 ) + sin( x / 1000. ) ) );
	
	// convert to gray
	float gr = 0.3 * c1 + 0.35 * c3 + 0.35 * c2;
	return vec4( gr / 6.0, gr / 6.0, gr / 6.0, 1 );
}

void main(void) 
{
	if ( isBG != 0.0 )
	{
		if ( plasmaSpeed > 0 )
			gl_FragColor = plasma2();
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
