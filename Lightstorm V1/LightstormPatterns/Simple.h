#ifndef Simple_h
#define Simple_h

#include "Arduino.h"

//==========================
//
//
//    Simple
//
//==========================

class Simple {
    
private:
    
public:
    
    int num_objects = 2;
    float objAmt[2];
    
    int mix_width = 5;
    int center = 0;

    int num_led = 0;
    
    void init(int n_led) {
        
        for(int i=0;i<num_objects;i++) {
            objAmt[i]=0.0;
        }
        
        num_led = n_led;
    }
    
    float get(int i) {
        float output = 1.0;

        if(i>center+mix_width) {
            objAmt[0] = 1.0;
            objAmt[1] = 0.0;
        } if(i>center-mix_width) {
            float x_fade = (i-(center-mix_width))/float(mix_width*2);
            objAmt[0] = x_fade;
            objAmt[1] = 1.0-x_fade;
        } else {
            objAmt[0] = 0.0;
            objAmt[1] = 1.0;
        }

        return output;
    }
    
    float get_obj_amt(int i) {
        return objAmt[i];
    }
    
    void update(bool trig, float lerp_speed) {
        center = (1.0-lerp_speed)*((num_led-1)+mix_width*4)-mix_width*2;
    }
    
    ////                     //
    /// GENERAL FUNCTIONS   ///
    //                     ////
    
    float lerp(float a, float b, float amount) {
        return a + (b-a)*amount;
    }
    
    float clip(float input, float lo, float hi) {
        float input_clip = 0.0;
        if(input<lo) {
            input_clip = lo;
        }else if(input>hi) {
            input_clip = hi;
        }else {
            input_clip = input;
        }
        return input_clip;
    }
};

#endif
