#ifndef Rand_h
#define Rand_h

#include "Arduino.h"

class Rand{

public:

	int clock_count = 0;
    
    int num_objects = 2;
    float objAmt[2];
    
    int num_led = 0;
    
    float env = 0.0;
    bool attack_phase = false;

    float sh_val = 0.0;
    float sh_obj = 0.0;
    float sh_val_buf[144];
    float sh_obj_buf[144];

    float count = 0.0;
    
    void init(int n_led) {
        
        for(int i=0;num_objects;i++) {
            objAmt[i]=0.0;
        }
        
        num_led = n_led;
    }
    
    float get(int i) {
        float output = 0.0;
        float idx_to_phase  = i/float(num_led-1);
        
        if(env>0.01) {
        	if(random(50)/50.0>0.9) {
        		sh_val = random(50)/50.0;
        		sh_val *= sh_val;
        		sh_obj = random(50)/50.0;
        	}
        	sh_val_buf[i] = sh_val;
        	sh_obj_buf[i] = sh_obj;
    	}

    	int walking_reader = int(i+count*(2*sh_obj_buf[i]-1))%72;

        output = sh_val_buf[walking_reader];
        objAmt[0] = sh_obj_buf[walking_reader];
        objAmt[1] = 1.0-sh_obj_buf[walking_reader];
        
        return output;
    }
    
    float get_obj_amt(int i) {
        return objAmt[i];
    }
    
    void update(bool trig, float lerp_speed_) {

        float env_speed = 0.25-lerp_speed_*0.18;

        if(trig)
            attack_phase = true;

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

        count += 1.0-lerp_speed_*0.8;
    }
    
private:

    ////                   //
    /// GENERAL FUNCTIONS ///
    //                   ////
    
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
