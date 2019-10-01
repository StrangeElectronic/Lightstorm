#ifndef Steps_h
#define Steps_h

#include "Arduino.h"

class Steps {

private:

public:

    PerlinNoise pn;

    float count = 0.0;

	int num_objects = 2;
	float objAmt[2];
    
    int num_led = 0;
    
    float env = 0.0;
    bool attack_phase = false;

    float peak = 0.0;
    int peak_dir = 1.0;
    
    void init(int n_led) {

        for(int i=0;num_objects;i++) {
            objAmt[i]=0.0;
        }
        
        num_led = n_led;
    }
    
    float get(int i) {
        float output = 0.0;
        float idx_to_phase  = i/float(num_led-1);

        float wave = 0.5-cos(idx_to_phase*TWO_PI)*0.5;//pn.pnoise(idx_to_phase*30.0,count*0.66,100);//
        wave *= peak;

        float steps = 4.0;
        wave = floor(wave*steps)/(steps-(1.0/steps));

        wave += count;

        wave = wave - floor(wave);

        output = wave;//1.0;
        objAmt[0]=wave;
        objAmt[1]=1.0-wave;

        return output;
    }
    
    float get_obj_amt(int i) {
        return objAmt[i];
    }
    
    void update(bool trig, float lerp_speed_) {

        peak = 1.5*peak_dir;

        count += (0.025+0.025*(1.0-lerp_speed_))+env*0.1;

        float env_speed = 0.05+(1.0-lerp_speed_)*0.15;

        if(trig) {
            attack_phase = true;
            if(peak_dir==1) {
                peak_dir = -1;
            } else {
                peak_dir = 1;
            }
        }

        if(attack_phase) {
            env += env_speed*3;
            if(env>=1.0) {
                attack_phase = false;
                env -= (env-1.0);
            }
        } else {
            env -= env_speed;
            if(env<0.0) 
                env = 0.0;
        }


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