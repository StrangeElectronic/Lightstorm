#ifndef Noisey_h
#define Noisey_h

#include "Arduino.h"


class Noisey {
private:
	int num_sparks;
	float sparks[14];
    int spark_obj_id[14];
	int num_led;
public:

	float decay = 0.7;

	float lo_thresh = 0.006;

	bool trig_mode = false;

	float width = 0.8;

	Noisey(int n_led) {

		num_sparks = 9;

		num_led = n_led;
		// sparks = (float*) realloc(sparks, num_led * sizeof(float));

		for(int i=0;i<num_sparks;i++) {
			sparks[i] = 0.0;//random(1000)/1000.0;
            spark_obj_id[i] = random(3);
		}
	}

	float get(int i) {
		return sparks[map(i,0,num_led-1,0,num_sparks-1)];
	}
    
    float get_obj_id(int i) {
        return spark_obj_id[map(i,0,num_led-1,0,num_sparks-1)];
    }

	void update(bool trig) {

		float decay_fin = 0.0;

		if(trig_mode) {

			decay_fin = 0.3-decay*0.299;

			for(int i=0; i<num_sparks; i++) {
				if(sparks[i]>=lo_thresh)
					sparks[i] = lerp(sparks[i],0.0,decay_fin*(1.0-random(5)*0.1));
			}

			if(trig) {
				bool spark_found = false;
				int n_trys = 0;
				int r_idx = 0;
				while(!spark_found && n_trys<num_sparks) {
					r_idx = random(num_sparks);
					if(sparks[r_idx]<lo_thresh)
						spark_found = true;
					n_trys++;
				}
				if(spark_found) {
					sparks[r_idx] = 1.0;
					spark_obj_id[r_idx] = random(3);
				}
			}
		} else {

			decay_fin = 0.7-decay*0.699;

			for(int i=0; i<num_sparks; i++) {
				sparks[i] = lerp(sparks[i],0.0,decay_fin*(1.0-random(5)*0.1));

                if(sparks[i]<lo_thresh) {
					sparks[i] = 1.0;
                    spark_obj_id[i] = random(3);
                }
			}
		}

		
	}

	void setDecay(float d) {
		decay = d;
	}

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
