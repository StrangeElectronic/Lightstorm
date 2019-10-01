#ifndef Corrupt_h
#define Corrupt_h

#include "Arduino.h"

class Corrupt {
private:
	Noisey *noise;
	int num_led;
public:
	int num_objects = 2;
    float objAmt[2];

    float thresh = 0.0;

    float env = 0.0;

	void init(int n_led) {
		num_led = n_led;
		noise = new Noisey(num_led);

		noise->lo_thresh = 0.001;

		noise->trig_mode = true;

		for(int i=0;i<num_objects;i++) {
            objAmt[i]=0.0;
        }
	}

	float get(int i) {

		float output = clip(noise->get(i),0.0,1.0);

		if(noise->get_obj_id(i)==2) {
            objAmt[0] = 0.0;
            objAmt[1] = 1.0;
        } 
        if(noise->get_obj_id(i)<2) {
            objAmt[0] = 1.0;
            objAmt[1] = 0.0;
        }
        

		return output;
	}

	float get_obj_amt(int i) {
        return objAmt[i];
    }

	void update(bool trig, float lerp_speed) {

		env = lerp(env,0.0,0.3-lerp_speed*0.29);

		if(trig)
			env = 1.0;

		noise->update(trig);
		noise->decay = lerp_speed;
	}

	// general functions

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