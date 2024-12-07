uniform float progress;

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex + vec4(vec2(0.0, 2.0) * (1.0 - progress), 0.0, 0.0);

    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

    gl_FrontColor = gl_Color;
}