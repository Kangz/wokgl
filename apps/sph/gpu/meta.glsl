#extension GL_EXT_gpu_shader4 : enable
in vec2 position;
layout(location = 0) out vec4 outColor;
uniform samplerBuffer particles;
uniform isamplerBuffer gridSize;
uniform float gridDim;
uniform isamplerBuffer gridOffset;
uniform isamplerBuffer gridArray;
uniform float threshold;

void main(){
    float squareSum = 0.0;

    //Iterate on every close particles
    vec2 fakePosition = (position * 0.999) - vec2(0.5 / gridDim);
    int startx = max(0, int(floor(fakePosition.x * gridDim)) - 2);
    int starty = max(0, int(floor(fakePosition.y * gridDim)) - 2);
    int endx = min(int(gridDim), startx + 2 + 4);
    int endy = min(int(gridDim), starty + 2 + 4);
    for(int x = startx; x < endx; x++){
        for(int y = starty; y < endy; y++){
            int cell = x + int(gridDim) * y;
            int size = texelFetchBuffer(gridSize, cell).r;
            int offset = texelFetchBuffer(gridOffset, cell).r;
            for(int j = 0; j < size; j++){
                int index = texelFetchBuffer(gridArray, offset + j).r;

                //Do meta balls with close particles
                vec2 particlePos = vec2(texelFetchBuffer(particles, index + PARTICLE_POS_X).r, texelFetchBuffer(particles, index + PARTICLE_POS_Y).r);
                vec2 diff = particlePos - position;
                float dist = dot(diff, diff);
                squareSum += 1.0 / max(0.0000001, dist);
                if(squareSum > threshold){
                    outColor = vec4(0.15, 0.3, 0.6, 1.0);
                    return;
                }
            }
        }
    }

    if(squareSum > threshold * 0.25){
        outColor = vec4(0.3, 0.5, 0.8, 1.0);
        return;
    }

    discard;
};
