#ifndef SwimCtrl_h
#define SwimCtrl_h

#include "Arduino.h"

class SwimCtrl {
	private:
		
	public:

		int num_objects = 2;
		float objAmt[2];

		int num_led = 0;

		float wave_phase = 0.0;

		void init(int n_led) {
	        
	        num_led = n_led;
		}

		float get(int i) {
			float output = 1.0;

			float idx_to_phase  = i/float(num_led-1);

			//idx_to_phase*TWO_PI

			float wave = 0.5+0.5*cos(i*0.08+wave_phase);

			objAmt[0] = wave;
			objAmt[1] = 1.0-wave;

			return output;
		}

		float get_obj_amt(int i) {
			return objAmt[i];
		}

		void update(bool trig, float lerp_speed_) {
			wave_phase = (1.0-lerp_speed_)*TWO_PI;
		}

};

#endif