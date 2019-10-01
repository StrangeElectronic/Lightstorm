#ifndef Steps2_h
#define Steps2_h

#include "Arduino.h"

class Steps2 {

private:

public:

    PerlinNoise pn;

    float count = 0.0;
    float speed_last = 0.0;
    float speed_dif_last = 0.0;

	int num_objects = 2;
	float objAmt[2];
    
    int num_led = 0;
    
    float env = 0.0;
    bool attack_phase = false;

    float peak = 1.5;
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

        float wave = pn.pnoise(idx_to_phase*30.0,count*0.6,100);
        wave *= peak;

        float steps = 4.0;
        wave = floor(wave*steps)/(steps-(1.0/steps));

        wave += count;

        wave = wave - floor(wave);

        output = 1.0;
        objAmt[0]=wave;
        objAmt[1]=1.0-wave;

        return output;
    }
    
    float get_obj_amt(int i) {
        return objAmt[i];
    }
    
    void update(bool trig, float lerp_speed_) {

    	float speed_dif = lerp_speed_-speed_last;

    	if(abs(speed_dif_last-speed_dif) > 0.005) {
    		speed_dif_last = speed_dif;
            count += abs(speed_dif);
    	} else {
    		speed_dif = 0.0;
    		speed_dif_last = speed_dif;
    	}

    	speed_last = lerp_speed_;

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