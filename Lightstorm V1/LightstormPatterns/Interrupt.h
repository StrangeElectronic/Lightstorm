#ifndef Interrupt_h
#define Interrupt_h

#include "Arduino.h"

class Interrupt{
	
public:
    
    int num_objects = 2;
    float objAmt[2];
    
    int num_led = 0;

    float env = 0.0;
    bool attack_phase = false;

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

        float x_mod = pn.pnoise(idx_to_phase*30.0,count*0.66,100);
        x_mod *= x_mod*x_mod;
        float x_mod_5s = floor(x_mod*5.0)/5.0;

        //output = sin(idx_to_phase*(4.5)+count*0.3+x_mod_5s*140*env);
        output = pn.pnoise(idx_to_phase*30.0,count*0.3+x_mod_5s*140*env,0);
        output *= output*output*output;
        output *= 2.5;
        output = clip(output,0.0,1.0);
        output = floor(output*5.0)/5.0;


        int x_mod_2s = 0;
        if(x_mod<0.35) {
        	x_mod_2s = 0;
        } else {
        	x_mod_2s = 1;
        }
        objAmt[0]=1.0;
        objAmt[1]=0.0;

        float col_n = pn.pnoise(idx_to_phase*30.0,count*0.3,20);
        col_n *= col_n;
        objAmt[0] = clip(1.0-col_n-x_mod_2s*env,0.0,1.0);
        objAmt[1] = clip(col_n+x_mod_2s*env,0.0,1.0);

        return output;
    }
    
    float get_obj_amt(int i) {
        return objAmt[i];
    }
    
    void update(bool trig, float lerp_speed_) {

        float env_speed = 0.0925-lerp_speed_*0.087;

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

        count += (1.0-lerp_speed_)*0.33+0.05+env*0.1;
    }

private:

	PerlinNoise pn;
	
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
