#ifndef VUCtrl_h
#define VUCtrl_h

#include "Arduino.h"

class VUCtrl {
	private:
		Mover *mover;

	public:

		int num_objects = 2;
	    float objAmt[2];
	    
	    int num_led = 0;

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
			float output = 1.0;

			float idx_to_phase  = i/float(num_led-1);
       		float mover_val = mover->check_mover(idx_to_phase*0.5+0.25);

       		objAmt[0] = mover_val;
       		objAmt[1] = 1.0-mover_val;

			return output;
		}

		float get_obj_amt(int i) {
			return objAmt[i];
		}

		void update(bool trig, float lerp_speed_) {
			mover->width = (1.0-lerp_speed_);
        	mover->pos = 0.5-mover->width*0.5;
		}

};

#endif