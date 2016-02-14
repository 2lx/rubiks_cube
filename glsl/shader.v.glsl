attribute vec3 cVertex;
attribute vec2 texCoord;
attribute float texIndex;
attribute vec2 scrVertex;

uniform mat4 mvp;
uniform float isBG;

varying vec2 f_texCoord;
flat varying float f_texIndex;
varying vec2 f_scrVertex;

void main(void) 
{
    if ( isBG == 0.0 )
        gl_Position = mvp * vec4(cVertex, 1.0);
    else gl_Position = vec4( scrVertex, 0.99, 1.0 );

    f_texCoord = texCoord;
    f_texIndex = texIndex;
    f_scrVertex = scrVertex;
}
