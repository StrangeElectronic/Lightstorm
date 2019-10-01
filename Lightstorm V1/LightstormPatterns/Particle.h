#ifndef Particle_h
#define Particle_h

#include "Arduino.h"

class Particle {

  public:
  Particle(float init_pos, float init_rate, float init_accel, float init_width) {
    pos = init_pos;
    rate = init_rate;
    accel = init_accel;
    width = init_width;
  }
  
  float check_particle(float p) {
    float val = 0.0;
    if(width > 0.0) {
      if(p>=pos && p<pos+width) {
        // val = 1.0;
        val = 0.5-0.5*cos(((p-pos)/width)*TWO_PI);
      }
    } else if(width < 0.0) {
      if(p<pos && p>pos+width) {
        // val = 1.0;
        val = 0.5-0.5*cos(((p-pos)/width)*TWO_PI);
      }
    }
    return val*peak;
  }

  void update() {
    if(alive) {
      pos += rate;
      rate += accel;

      if(width > 0.0) {
        if(pos>1.0 || pos+width<0.0) {
          alive = false;
        }
      } else if(width < 0.0) {
        if(pos+width>1.0 || pos<0.0) {
          alive = false;
        }
      }
    }
  }

  void fire(float fire_pos, float fire_rate, float fire_accel, float fire_width) {
    pos = fire_pos;
    rate = fire_rate;
    accel = fire_accel;
    width = fire_width;
    alive = true;
  }
   
  float peak = 1.0;
  float pos = 0.0;
  float width = 0.03;
  float rate = 0.0001;
  float accel = 0.00;
  bool alive = false;
};

#endif
