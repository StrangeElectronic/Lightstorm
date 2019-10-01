#ifndef Flow_h
#define Flow_h

#include "Arduino.h"

class Flow {

public:
	int num_objects = 2;
    float objAmt[2];
    
    int num_led = 0;

    float count = 0.0;
    float speed_last = 0.0;
    float speed_dif_last = 0.0;


    void init(int n_led) {
        
        for(int i=0;num_objects;i++) {
            objAmt[i]=0.0;
        }
        
        num_led = n_led;
    }

    float get(int i) {
        float output = 1.0;
        float idx_to_phase  = i/float(num_led-1);

        float mod = pn.pnoise(idx_to_phase*25.0,count*20,200);
        float noise = pn.pnoise(idx_to_phase*30.0+mod*20,count*20,100);

        noise = noise*noise*noise;
        noise *= 1.5;
        noise -= 0.;

        noise = clip(noise,0.0,1.0);

        output = clip(0.5+noise*0.75,0.0,1.0);

		objAmt[0]=noise;
        objAmt[1]=1.0-noise;

        return output;
    }

    float get_obj_amt(int i) {
        return objAmt[i];
    }
    
    void update(bool trig, float lerp_speed_) {

    	float speed_dif = lerp_speed_-speed_last;

    	if(abs(speed_dif_last-speed_dif) > 0.008) {
    		speed_dif_last = speed_dif;
    	} else {
    		speed_dif = 0.0;
    		speed_dif_last = speed_dif;
    	}

    	count += abs(speed_dif);

    	speed_last = lerp_speed_;
    }



private:
	PerlinNoise pn;

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