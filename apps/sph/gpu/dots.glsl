#extension GL_EXT_gpu_shader4 : enable
in vec2 position;
layout(location = 0) out vec4 outColor;
uniform samplerBuffer particles;
uniform isamplerBuffer gridSize;
uniform float gridDim;
uniform isamplerBuffer gridOffset;
uniform isamplerBuffer gridArray;

void main(){
   vec2 fakePosition = (position * 0.999) - vec2(0.5 / gridDim);
   int startx = max(0, int(floor(fakePosition.x * gridDim)));
   int starty = max(0, int(floor(fakePosition.y * gridDim)));
   int endx = min(int(gridDim), startx + 2);
   int endy = min(int(gridDim), starty + 2);

   for(int x = startx; x < endx; x++){
       for(int y = starty; y < endy; y++){
           int cell = x + int(gridDim) * y;
           int size = texelFetchBuffer(gridSize, cell).r;
           int offset = texelFetchBuffer(gridOffset, cell).r;
           for(int j = 0; j < size; j++){
               int index = texelFetchBuffer(gridArray, offset + j).r;
               vec2 particlePos = vec2(texelFetchBuffer(particles, index + PARTICLE_POS_X).r, texelFetchBuffer(particles, index + PARTICLE_POS_Y).r);
               vec2 diff = particlePos - position;
               float dist = dot(diff, diff);
               if(dist < 0.000002){
                   float weight = abs(texelFetchBuffer(particles, index + PARTICLE_PRESSURE).r / 10.0);
                   float normalizedWeight = 1.1 - 1 / (1 + weight);
                   outColor = vec4(vec3(normalizedWeight), 1.0);
                   return;
               }
           }
       }
   }
};
