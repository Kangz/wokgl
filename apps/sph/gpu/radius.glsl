in vec2 position;
layout(location = 0) out vec4 outColor;
uniform float x;
uniform float y;
uniform float r;
uniform float time;

const float pi = 3.1415;

void main(){
    vec2 diff = position - vec2(x, y);
    float dist = length(diff);
    float angle = atan(diff.x, diff.y);

    if(abs(dist - r * 0.8) < r / 30 && int(- 100 - time / 200 + angle / pi / 2 * 30) % 2 == 1){
        outColor = vec4(0.5, 0.5, 0.5, 1.0);
    }else if(abs(dist - r * 0.3) < r / 40 && int(100 + time / 100 + angle / pi / 2 * 20) % 2 == 1){
        outColor = vec4(0.5, 0.5, 0.5, 1.0);
    }else{
        outColor = vec4(0.0);
    }
}
