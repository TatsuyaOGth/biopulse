#ifdef GL_ES
precision lowp float;
#endif

uniform vec2 mouse;
uniform vec2 resolution;
uniform float time;
uniform float size;

#define fmod 0.1

void main()
{
    float x = resolution.x / 2.0;
    float y = resolution.y / 2.0;
    vec2 pos = vec2(x, y);
    float dist = length(gl_FragCoord.xy - pos);
    float color = pow(size / dist, 3.0);
	gl_FragColor = vec4(vec3(color), 1.0);
}
