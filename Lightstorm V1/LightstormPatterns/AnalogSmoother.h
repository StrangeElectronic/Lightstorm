#ifndef AnalogSmoother_h
#define AnalogSmoother_h

#include "Arduino.h"

class AnalogSmoother {
	public:
  	AnalogSmoother(int init_val, int bsize=3) {
  		last_out = init_val;
  		last_in = init_val;
  		last_av = init_val;
  		buf_size = bsize;
  	}

  	int update(int new_val) {
  		// add value to the average buffer
		buf[write_head] = new_val;

		// update the write head
		write_head++;
		if(write_head==buf_size) {
			write_head -= buf_size;
		}

		// calc average
		// only calc if input is less than large_delta 
		int average = last_av;
		if(abs(new_val-last_in) < large_delta) {
			int sum = 0;
			float weight = 1.0;
			float w_sum = 0.0;
			for(int i=0;i<buf_size;i++) {
				sum += buf[i]*weight;
				w_sum += 1.0;
			}
			average = round(float(sum)/buf_size);
		}

		// scale and clip average to bring back to 10bit
		// average = map(average,min_in,max_in,0,1023);
		if(average > 1023)
			average = 1023;
		if(average < 0)
			average = 0;

		// if new avereage is greater than 1 increment different from last average
		// than use it else throw out the new average
		if(abs(average-last_av)>2) {
			// greater than 1 increment different use average
			last_av = average;
		} else {
			// throw out the new average
			average = last_av;
		}
		
		last_in = new_val;

		return average;
	}

	int buf_size = 3;
	int buf[10];
	int write_head = 0;

    int min_in = 60;
    int max_in = 970;

	int min_dif = 2;
	int large_delta = 150;
	int last_out = 0;
	int last_in = 0;
	int last_av = 0;
};

#endif
