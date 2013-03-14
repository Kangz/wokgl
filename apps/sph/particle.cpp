#include <sstream>

#include "particle.hpp"

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

float _mass;
float _radius;
float _celerity;
float _rho0;
float _timestep;

std::string getParticleHeader(){
    std::ostringstream o;
    o << PARTICLE_HEADER;
    o << "#define MASS " << _mass << std::endl;
    o << "#define RADIUS " << _radius << std::endl;
    o << "#define CELERITY " << _celerity << std::endl;
    o << "#define RHO0 " << _rho0 << std::endl;
    o << "#define TIMESTEP " << _timestep << std::endl;
    return o.str();
}

void setMass(float mass){
    _mass = mass;
}

void setRadius(float radius){
    _radius = radius;
}

void setCelerity(float celerity){
    _celerity = celerity;
}

void setRho0(float rho0){
    _rho0 = rho0;
}

void setTimestep(float timestep){
    _timestep = timestep;
}

