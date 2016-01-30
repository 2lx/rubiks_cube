varying vec2 f_texcoord;
uniform sampler2D mytextureRed;
uniform sampler2D mytextureYellow;

void main(void) {
//  vec2 flipped_texcoord = vec2(f_texcoord.x, 1.0 - f_texcoord.y);
//  if ( f_vertcoord.x > 0 )
	  gl_FragColor = texture2D(mytextureYellow, f_texcoord);  
//  else gl_FragColor = texture2D(mytextureYellow, flipped_texcoord);  

//gl_FragColor = vec4(f_texcoord, 0, 1.0);
}
