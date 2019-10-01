#ifndef SpinInv_h
#define SpinInv_h

#include "Arduino.h"

//==========================
//
//
//    Spin
//
//==========================

class SpinInv {
    
private:
    Mover *mover;
    Mover *mover2;
public:
    int clock_count = 0;
    
    int num_objects = 2;
    float objAmt[2];
    
    int num_led = 0;
    
    float env = 0.0;
    bool attack_phase = false;
    
    void init(int n_led) {
        mover = new Mover(0.0,0.002,0.1);
        mover2 = new Mover(0.0,0.002,0.1);
        
        mover->peak = 1.0;
        mover2->peak = 1.0;
        mover->wave_mode = true;
        mover2->wave_mode = true;
        mover->rate = 0.0;
        mover2->rate = 0.0;
        mover->width = 0.3;
        mover2->width = 0.3;
        mover->pos = 0.5-mover->width*0.5;
        mover2->pos = 1.0-mover2->width*0.5;
        
        for(int i=0;i<num_objects;i++) {
            objAmt[i]=0.0;
        }
        
        num_led = n_led;
    }
    
    float get(int i) {
        float output = 0.0;
        
        float idx_to_phase  = i/float(num_led-1);
        float mover_val = mover->check_mover(idx_to_phase);
        float mover2_val = mover2->check_mover(idx_to_phase);
        
        // mixed output
        float val_sum = mover_val+mover2_val;
        float one_val_sum = 0.0;
        if(val_sum>0.0)
            one_val_sum = 1/val_sum;

        output = clip(val_sum,0.0,1.0);

        if(one_val_sum==0.0) {
            objAmt[0] = 0.0;
            objAmt[1] = 0.0;
        } else {
            objAmt[0] = mover_val*one_val_sum;
            objAmt[1] = mover2_val*one_val_sum;
        }
        
        // one or the other output
        // if(mover_val>0.0) {
        //     output = mover_val;
        //     objAmt[0] = 1.0;
        //     objAmt[1] = 0.0;
        // } else if(mover2_val>0.0) {
        //     output = mover2_val;
        //     objAmt[0] = 0.0;
        //     objAmt[1] = 1.0;
        // } else {
        //     output = 0.0;
        //     objAmt[0] = 0.0;
        //     objAmt[1] = 0.0;
        // }
        
        return output;
    }
    
    float get_obj_amt(int i) {
        return objAmt[i];
    }
    
    void update(bool trig, float lerp_speed_) {

        if(trig)
            attack_phase = true;
        
        float env_speed = 0.15;
        if(attack_phase) {
            env += env_speed*4;
            if(env>=1.0) {
                attack_phase = false;
                env -= (env-1.0);
            }
        } else {
            env -= env_speed;
        }
        
        env = clip(env,0.0,1.0);
        
        mover->rate = -(0.005+0.01*(1.0-lerp_speed_))-0.105*(1.0-lerp_speed_)*env;
        mover2->rate = -(0.005+0.01*(1.0-lerp_speed_))-0.125*(1.0-lerp_speed_)*env;

        mover->update();
        mover2->update();
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
