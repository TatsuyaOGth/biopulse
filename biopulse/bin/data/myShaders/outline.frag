uniform sampler2DRect image;
varying vec3 pos;
uniform float th;

float convertGray(vec4 col)
{
    return (0.299 * col.r) + (0.587 * col.g) + (0.114 * col.b);
}

void main (void)
{
    
    vec4 col  = texture2DRect(image, vec2(pos.x, pos.y));
    vec4 offx = texture2DRect(image, vec2(pos.x + 1.0, pos.y));
    vec4 offy = texture2DRect(image, vec2(pos.x, pos.y + 1.0));
    
    float gray  = convertGray(col);
    float goffx = convertGray(offx);
    float goffy = convertGray(offy);
    
    float dst = abs(gray - goffx) + abs(gray - goffy);
    
    if (dst > th) {
        dst = 1.0;
    } else {
        dst = 0.0;
    }
    
//    float r = abs(col.r - offx.r) + abs(col.r - offy.r);
//    float g = abs(col.g - offx.g) + abs(col.g - offy.g);
//    float b = abs(col.b - offx.b) + abs(col.b - offy.b);
    
    gl_FragColor.a = 1.0;
    gl_FragColor.rgb = vec3(dst, dst, dst);
    
    
}
