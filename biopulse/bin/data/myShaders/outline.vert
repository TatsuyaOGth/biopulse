

//void main(void)
//{
//    gl_Position    = gl_ModelViewProjectionMatrix * gl_Vertex;
//    gl_TexCoord[0] = gl_MultiTexCoord0;
//    gl_FrontColor  = gl_Color;
//}

varying vec3 pos;
uniform float th;

void main(void)
{
    pos = gl_Vertex.xyz;
    gl_Position = ftransform();
}