#include <string>

#define PARTICLE_SIZE 8
#define PARTICLE_FULLSIZE (PARTICLE_SIZE * sizeof(float))

#define PARTICLE_POS 0
#define PARTICLE_POS_X 0
#define PARTICLE_POS_Y 1
#define PARTICLE_SPEED 2
#define PARTICLE_SPEED_X 2
#define PARTICLE_SPEED_Y 3
#define PARTICLE_AVERAGE_WEIGHT 4
#define PARTICLE_PRESSURE 5
#define PARTICLE_ACCEL_X 6
#define PARTICLE_ACCEL_Y 7

const std::string PARTICLE_HEADER = ""
"#define PARTICLE_SIZE 8\n"

"#define PARTICLE_POS 0\n"
"#define PARTICLE_POS_X 0\n"
"#define PARTICLE_POS_Y 1\n"
"#define PARTICLE_SPEED 2\n"
"#define PARTICLE_SPEED_X 2\n"
"#define PARTICLE_SPEED_Y 3\n"
"#define PARTICLE_AVERAGE_WEIGHT 4\n"
"#define PARTICLE_PRESSURE 5\n"
"#define PARTICLE_ACCEL_X 6\n"
"#define PARTICLE_ACCEL_Y 7\n"
;
