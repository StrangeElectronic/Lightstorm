#ifndef Pulse_h
#define Pulse_h

#include "Arduino.h"

//==========================
//
//
//    Pulse
//
//==========================

class Pulse {
    
private:
    Mover *mover;
    Mover *mover2;
public:
    int clock_count = 0;
    
    int num_objects = 2;
    float objAmt[2];
    
    int num_led = 0;
    
    int frameCount = 0;
    
    void init(int n_led) {
        mover = new Mover(0.0,0.002,0.1);
        mover2 = new Mover(0.0,0.002,0.1);
        
        mover->peak = 0.0;
        mover2->peak = 0.0;
        mover->wave_mode = false;
        mover2->wave_mode = false;
        mover->rate = 0.0;
        mover2->rate = 0.0;
        mover->width = 0.5;
        mover2->width = 0.5;
        mover->pos = 0.5-mover->width*0.5;
        mover2->pos = 1-mover2->width*0.5;
        
        for(int i=0;num_objects;i++) {
            objAmt[i]=0.0;
        }
        
        num_led = n_led;
    }
    
    float get(int i) {
        float output = 0.0;
        
        float idx_to_phase  = i/float(num_led-1);
        float mover_val = mover->check_mover(idx_to_phase);
        float mover2_val = mover2->check_mover(idx_to_phase);
        
        if(mover_val>0.0) {
            output = mover_val;
            objAmt[0] = 1.0;
            objAmt[1] = 0.0;
        } else if(mover2_val>0.0) {
            output = mover2_val;
            objAmt[0] = 0.0;
            objAmt[1] = 1.0;
        } else {
            output = 0.0;
            objAmt[0] = 0.0;
            objAmt[1] = 0.0;
        }
        
        return output;
    }
    
    float get_obj_amt(int i) {
        return objAmt[i];
    }
    
    void update(bool trig, float lerp_speed) {

        lerp_speed = 0.45-lerp_speed*0.43;
        
        if(trig) {
            if(random(10)>6) {
                mover->peak = 1.0;
                mover2->peak = 1.0;
            } else {
                if(clock_count%2==0) {
                    mover->peak = 1.0;
                } else {
                    mover2->peak = 1.0;
                }
            }
            
            clock_count++;
        }
        
        mover->peak = lerp(mover->peak,0.0,lerp_speed);
        mover2->peak = lerp(mover2->peak,0.0,lerp_speed);
        
        mover->pos = 0.5+sin(frameCount*0.006)*0.15-mover->width*0.5;
        mover2->pos = 1.0+sin(frameCount*0.006)*0.15-mover2->width*0.5;
        
        frameCount++;
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
