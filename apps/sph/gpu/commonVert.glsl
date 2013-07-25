in vec2 in_Position;
out vec2 position;

void main(){
   gl_Position = vec4(in_Position, 0.0, 1.0);
   position = gl_Position.xy / 2  + vec2(0.5f);
};
