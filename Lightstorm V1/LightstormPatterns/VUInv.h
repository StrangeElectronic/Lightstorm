#ifndef VUInv_h
#define VUInv_h

#include "Arduino.h"

class VUInv {
    
private:
    Mover *mover;
public:
    int clock_count = 0;
    
    int num_objects = 2;
    float objAmt[2];
    
    int num_led = 0;
    
    float env = 0.0;
    bool attack_phase = false;
    
    void init(int n_led) {
        mover = new Mover(0.0,0.002,0.1);
        
        mover->peak = 1.0;
        mover->wave_mode = false;
        mover->rate = 0.0;
        mover->width = 0.0;
        
        for(int i=0;num_objects;i++) {
            objAmt[i]=0.0;
        }
        
        num_led = n_led;
    }
    
    float get(int i) {
        float output = 0.0;
        
        float idx_to_phase  = i/float(num_led-1);
        float mover_val = mover->check_mover(idx_to_phase);
        
        output = mover_val;
        objAmt[0] = 1.0-env;
        objAmt[1] = env;
        
        return output;
    }
    
    float get_obj_amt(int i) {
        return objAmt[i];
    }
    
    void update(bool trig, float lerp_speed_) {

        float lerp_speed = 0.25-lerp_speed_*0.23;

        if(trig)
            attack_phase = true;

        if(attack_phase) {
            env += lerp_speed*4;
            if(env>=1.0) {
                attack_phase = false;
                env -= (env-1.0);
            }
        } else {
            env -= lerp_speed;
        }
        
        env = clip(env,0.0,1.0);

        mover->width = env;
        mover->pos = 0.5-mover->width*0.5;
    }
    
    ////                     //
    /// GENERAL FUNCTIONS ///
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
