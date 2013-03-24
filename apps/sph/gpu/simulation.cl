#define EPSILON 0.0001f
#define FORGRID(posx, posy, particle_result)\
   int startx = max(0, (int)floor(((posx) - RADIUS) * gridDim) - 1000);\
   int starty = max(0, (int)floor(((posy) - RADIUS) * gridDim) - 1000);\
   int endx = min(gridDim, (int)ceil(((posx) + RADIUS) * gridDim) + 1000);\
   int endy = min(gridDim, (int)ceil(((posy) + RADIUS) * gridDim) + 1000);\
   for(int x = startx; x < endx; x++){\
       for(int y = starty; y < endy; y++){\
           int cell = x + gridDim * y;\
           int size = gridSize[cell];\
           int offset = gridOffset[cell];\
           for(int l = 0; l < size; l++){\
               int particle_index = gridArray[offset + l];\
               if(particle_index != get_global_id(0)){\
                   (particle_result) = particle_index;\

#define ENDFORGRID }}}}

kernel void advect(read_only global float* oldParticles, write_only global float* newParticles){
   int i = get_global_id(0) * PARTICLE_SIZE;
   float accelx = oldParticles[i + PARTICLE_ACCEL_X];
   float accely = oldParticles[i + PARTICLE_ACCEL_Y];
   float speedx = oldParticles[i + PARTICLE_SPEED_X] + accelx * MASS * TIMESTEP;
   float speedy = oldParticles[i + PARTICLE_SPEED_Y] + accely * MASS * TIMESTEP - 0.0 * TIMESTEP / 100;
   float newposx = oldParticles[i + PARTICLE_POS_X] + speedx * TIMESTEP;
   float newposy = oldParticles[i + PARTICLE_POS_Y] + speedy * TIMESTEP;
/*
   if(newposx < 0.0f + EPSILON){ newposx = 0.0f + EPSILON; speedx = + EPSILON;}
   if(newposx > 1.0f - EPSILON){ newposx = 1.0f - EPSILON; speedx = - EPSILON;}
   if(newposy < 0.0f + EPSILON){ newposy = 0.0f + EPSILON; speedy = + EPSILON;}
   if(newposy > 1.0f - EPSILON){ newposy = 1.0f - EPSILON; speedy = - EPSILON;}
*/
   if(newposx < 0.0f + EPSILON){ newposx = 0.0f + EPSILON; speedx = - speedx / 2;}
   if(newposx > 1.0f - EPSILON){ newposx = 1.0f - EPSILON; speedx = - speedx / 2;}
   if(newposy < 0.0f + EPSILON){ newposy = 0.0f + EPSILON; speedy = - speedy / 2 ;}
   if(newposy > 1.0f - EPSILON){ newposy = 1.0f - EPSILON; speedy = - speedy / 2;}

   newParticles[i + PARTICLE_POS_X] = newposx;
   newParticles[i + PARTICLE_POS_Y] = newposy;
   newParticles[i + PARTICLE_SPEED_X] = speedx;
   newParticles[i + PARTICLE_SPEED_Y] = speedy;
}

float W(float dist, float h){
   const float x = dist / h;
   if(x < 0.5f){
       return 1 + 6 * x * x * (x - 1);
   }else if(x < 1.0f){
       const float y = 1 - x;
       return 2 * y * y * y;
   }else{
       return 0;
   }
}

float dW(float dist, float h){
   const float x = dist / h;
   if(x < 0.5f){
       return (18 * x - 12) * x / h;
   }else if(x < 1.0f){
       const float y = 1 - x;
       return - 4 / h * y * y;
   }else{
       return 0;
   }
}

float laplacianW(float dist, float h){
   const float x = dist / h;
   float result;
   if(x < 0.5f){
       result = (36 * x - 12) / h / h;
   }else if(x < 1.0f){
       const float y = 1 - x;
       result = - 4 / h / h * y;
   }else{
       return 0;
   }
   return result + dW(dist, h) / max(0.000001, x);
}


kernel void computeAverageWeight(global float* particles, read_only global int* gridSize,
       read_only global int* gridOffset, read_only global int* gridArray, int gridDim){
   int i = get_global_id(0) * PARTICLE_SIZE;
   float posx = particles[i + PARTICLE_POS_X];
   float posy = particles[i + PARTICLE_POS_Y];
   float weight = 1.0f; //Count its own weight
   int index;
   FORGRID(posx, posy, index){
       float dx = posx - particles[index + PARTICLE_POS_X];
       float dy = posy - particles[index + PARTICLE_POS_Y];
       float dist = sqrt(dx * dx + dy * dy);
       weight += W(dist, RADIUS);
   }ENDFORGRID
   particles[i + PARTICLE_AVERAGE_WEIGHT] = weight;
   particles[i + PARTICLE_PRESSURE] = CELERITY * CELERITY * (weight - RHO0);
}

kernel void computeAccel(global float* particles, read_only global int* gridSize,
       read_only global int* gridOffset, read_only global int* gridArray, int gridDim){
   int i = get_global_id(0) * PARTICLE_SIZE;
   float posx = particles[i + PARTICLE_POS_X];
   float posy = particles[i + PARTICLE_POS_Y];
   float own_w = particles[i + PARTICLE_AVERAGE_WEIGHT];
   float own_p = particles[i + PARTICLE_PRESSURE];
   int index;
   float accelx = 0.0f;
   float accely = 0.0f;
   FORGRID(posx, posy, index){
       float dx = posx - particles[index + PARTICLE_POS_X];
       float dy = posy - particles[index + PARTICLE_POS_Y];
       float other_w = particles[index + PARTICLE_AVERAGE_WEIGHT];
       float other_p = particles[index + PARTICLE_PRESSURE];
       float dist = sqrt(dx * dx + dy * dy);
       float factor = - (own_p / own_w / own_w + other_p / other_w / other_w) / max(0.00000001, dist) * dW(dist, RADIUS);
       accelx += dx * factor;
       accely += dy * factor;
   }ENDFORGRID
   particles[i + PARTICLE_ACCEL_X] = accelx;
   particles[i + PARTICLE_ACCEL_Y] = accely;
}

