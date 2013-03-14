kernel void zero(write_only global int* buffer){
   buffer[get_global_id(0)] = 0;
}

kernel void count(read_only global float* particles, volatile global int* count,
               const int gridSize){
   int i = get_global_id(0) * PARTICLE_SIZE;
   int x = floor(particles[i + PARTICLE_POS_X] * gridSize);
   int y = floor(particles[i + PARTICLE_POS_Y] * gridSize);
   atomic_add(&count[x + gridSize * y], 1);
}

kernel void place(read_only global float* particles, read_only global int* offset,
               volatile global int* counters, write_only global int* array,
               const int gridSize){
   int i = get_global_id(0) * PARTICLE_SIZE;
   int x = floor(particles[i + PARTICLE_POS_X] * gridSize);
   int y = floor(particles[i + PARTICLE_POS_Y] * gridSize);
   int cell = x + gridSize * y;
   int number = atomic_add(&counters[cell], 1);
   array[offset[cell] + number] = i;
}
