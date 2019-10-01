#ifndef Mover_h
#define Mover_h

#include "Arduino.h"

class Mover {

  public:
  Mover(float init_pos, float init_rate, float init_width) {
    pos = init_pos;
    rate = init_rate;
      width = init_width;
  }
  
  float check_mover(float p) {
    float val = 0.0;
    if(width > 0.0) {
      if(p>=pos && p<pos+width) {
        if(wave_mode) {
          val = 0.5-0.5*cos(((p-pos)/width)*TWO_PI);
        } else {
          val = 1.0;
        }
      }
      if(pos+width>1.0 && p<pos+width-1.0) {
        if(wave_mode) {
          val = 0.5-0.5*cos(((p-(pos-1.0))/width)*TWO_PI);
        } else {
          val = 1.0;
        }
      }
    } else if(width < 0.0) {
      if(p<pos && p>pos+width) {
        if(wave_mode) {
          val = 0.5-0.5*cos(((p-pos)/width)*TWO_PI);
        } else {
          val = 1.0;
        }
      }
      if(pos+width<0.0 && p>pos+width+1.0) {
        if(wave_mode) {
          val = 0.5-0.5*cos(((p-(pos+1.0))/width)*TWO_PI);
        } else {
          val = 1.0;
        }
      }
    }
    return val*peak;
  }

  void update() {
    pos += rate;
    if(pos>1.0) {
      pos -= 1.0;
    }
    if(pos<0.0) {
        pos += 1.0;
    }
  }

  // light mover setup 
  float peak = 1.0;
  float pos = 0.0;
  float width = 0.03;
  float rate = 0.0001;
  bool wave_mode = true;
};

#endif
