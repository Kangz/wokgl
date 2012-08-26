#version 410
in  vec2 in_Position;
 
out vec3 position;
void main(void) {
    gl_Position = vec4(in_Position.x, in_Position.y, 0.0, 1.0);
    position = gl_Position.xyz;
}
