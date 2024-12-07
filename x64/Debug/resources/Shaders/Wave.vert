uniform float phase;
uniform vec2 amplitude;

void main()
{
    vec4 vertex = gl_Vertex;
    vertex.x += cos(gl_Vertex.y * 0.6 + phase * 3.0) * amplitude.x;    
    vertex.y += sin(gl_Vertex.x * 0.1 + phase * 4.0) * amplitude.y;
              
    gl_Position = gl_ModelViewProjectionMatrix * vertex;
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    gl_FrontColor = gl_Color;
}