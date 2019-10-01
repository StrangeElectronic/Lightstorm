#ifndef VU_h
#define VU_h

#include "Arduino.h"

class VU {
    
private:
    Mover *mover;
    PerlinNoise pn;
public:
    int clock_count = 0;
    
    int num_objects = 2;
    float objAmt[2];
    
    int num_led = 0;
    
    float env = 0.0;
    bool attack_phase = false;

    float count = 0.0;
    
    void init(int n_led) {
        mover = new Mover(0.0,0.002,0.1);
        
        mover->peak = 1.0;
        mover->wave_mode = true;
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
        
        float noise = pn.pnoise(idx_to_phase*130+env*100,count,0)*2;
        float noise2 = pn.pnoise(idx_to_phase*30+env*10,count,0)*1.5;
        noise *= noise;
        noise -= noise2;
        noise = clip(noise,0.0,1.0);

        float amb_noise = pn.pnoise(idx_to_phase*25,count*0.5,20)*(0.9-env);
        output = 1-mover_val*noise-amb_noise;
        objAmt[0] = mover_val;
        objAmt[1] = 1.0-mover_val;
        
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

        mover->width = env*2.0;

        float phase = count*0.1-floor(count*0.1);
        mover->pos = phase-mover->width*0.5;

        count += 0.33;
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
