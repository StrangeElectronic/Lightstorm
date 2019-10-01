#ifndef VWB_h
#define VWB_h

#include "Arduino.h"

//==========================
//
//
//    variable width bounderies
//
//==========================

class VWB {
    
private:
    int N_spaces = 5;
    float bounds[5-1];
    float lerp_bounds[5-1];
    float B_center[5-1];
    float weights[5];
    float lerp_weights[5];
    float motion_dir[5];
    float B_range = 1.0/5.0;

    int n_phase, gate_counter, gate_len;
public:
    
    int num_objects = 2;
    float objAmt[2];
    
    int num_led = 0;
    
    void init(int n_led) {

        n_phase = 0;
        gate_counter = 100000;

        for(int i=0;i<N_spaces-1;i++) {
            B_center[i] = B_range*(i+1);
            bounds[i] = B_center[i];
            lerp_bounds[i] = B_center[i];
        }
        
        for(int i=0;i<num_objects;i++) {
            objAmt[i]=0.0;
        }
        
        for(int i=0;i<N_spaces;i++) {
            weights[i] = random(11)/10.0;
            lerp_weights[i] = weights[i];
            motion_dir[i] = random(21)*0.001;
        }

        num_led = n_led;
    }
    
    float get(int i) {
        float output = 0.0;
        
        float idx_to_phase  = i/float(num_led-1);

        float color_dif = 3;

        if(idx_to_phase>=get_boundery(0) && idx_to_phase<get_boundery(1)) {
            float phase_in_boundry = (idx_to_phase-get_boundery(0))/get_width(0);

            output = 0.5-0.5*cos(phase_in_boundry*TWO_PI);
            output *= output*output;

            objAmt[0] = clip(1.0-(lerp_weights[0]*color_dif-(color_dif-1.0)*0.5),0.0,1.0);
            objAmt[1] = clip(lerp_weights[0]*color_dif-(color_dif-1.0)*0.5,0.0,1.0);
        } else if(idx_to_phase>=get_boundery(1) && idx_to_phase<get_boundery(2)) {
            float phase_in_boundry = (idx_to_phase-get_boundery(1))/get_width(1);

            output = 0.5-0.5*cos(phase_in_boundry*TWO_PI);
            output *= output*output;

            objAmt[0] = clip(1.0-(lerp_weights[1]*color_dif-(color_dif-1.0)*0.5),0.0,1.0);
            objAmt[1] = clip(lerp_weights[1]*color_dif-(color_dif-1.0)*0.5,0.0,1.0);
        } else if(idx_to_phase>=get_boundery(2) && idx_to_phase<get_boundery(3)) {
            float phase_in_boundry = (idx_to_phase-get_boundery(2))/get_width(2);

            output = 0.5-0.5*cos(phase_in_boundry*TWO_PI);
            output *= output*output;

            objAmt[0] = clip(1.0-(lerp_weights[2]*color_dif-(color_dif-1.0)*0.5),0.0,1.0);
            objAmt[1] = clip(lerp_weights[2]*color_dif-(color_dif-1.0)*0.5,0.0,1.0);
        } else if(idx_to_phase>=get_boundery(3) && idx_to_phase<get_boundery(4)) {
            float phase_in_boundry = (idx_to_phase-get_boundery(3))/get_width(3);

            output = 0.5-0.5*cos(phase_in_boundry*TWO_PI);
            output *= output*output;

            objAmt[0] = clip(1.0-(lerp_weights[3]*color_dif-(color_dif-1.0)*0.5),0.0,1.0);
            objAmt[1] = clip(lerp_weights[3]*color_dif-(color_dif-1.0)*0.5,0.0,1.0);
        } else if(idx_to_phase>=get_boundery(4) && idx_to_phase<get_boundery(5)) {
            float phase_in_boundry = (idx_to_phase-get_boundery(4))/get_width(4);

            output = 0.5-0.5*cos(phase_in_boundry*TWO_PI);
            output *= output*output;

            objAmt[0] = clip(1.0-(lerp_weights[4]*color_dif-(color_dif-1.0)*0.5),0.0,1.0);
            objAmt[1] = clip(lerp_weights[4]*color_dif-(color_dif-1.0)*0.5,0.0,1.0);
        }

        return output;
    }
    
    float get_obj_amt(int i) {
        return objAmt[i];
    }
    
    void update(bool trig, float lerp_speed_) {

        float lerp_speed = 0.04 + (1.0-lerp_speed_)*0.3;

        if(trig) {
            for(int i=0;i<N_spaces;i++) {
                float r = random(11)/10.0;
                if(weights[i] < 0.33) {
                    weights[i] = 0.33+r*0.66;
                } else if(weights[i] >= 0.33 && weights[i] < 0.66) {
                    if(r<0.5) {
                        weights[i] = r*0.66;
                    } else {
                        weights[i] = 0.33+r*0.66;
                    }
                } else {
                    weights[i] = r*0.66;
                }
            }
        }

        for(int i=0;i<N_spaces;i++) {

            weights[i] += motion_dir[i]*(1.0-lerp_speed_);
            if(weights[i]>1.0) {
                weights[i] = 1.0;
                motion_dir[i] *= -1;
            } else if(weights[i]<0.0) {
                weights[i] = 0.0;
                motion_dir[i] *= -1;
            }

            lerp_weights[i] = lerp(lerp_weights[i],weights[i],lerp_speed);
        }

        for(int i=0;i<N_spaces-1;i++) {    
            float b_mod = 0.0;

            b_mod = lerp_weights[i]*B_range*0.5-lerp_weights[i+1]*B_range*0.5;

            bounds[i] = B_center[i]+b_mod;
            lerp_bounds[i] = lerp(lerp_bounds[i],bounds[i],1.0);
        }
            
    }

    float get_boundery(int i) {
        float out;

        if(i==0) {
            out = 0;    
        } else if(i==N_spaces) {
            out = 1.0;
        } else {
            out = lerp_bounds[i-1];
        }

        return out;
    }

    float get_width(int i) {
        float out = 0.0;

        if(i >= 0 && i < N_spaces) {

            if(i == 0) {
                out = lerp_bounds[i];
            } else if(i == N_spaces-1) {
                out = 1.0-lerp_bounds[i-1];
            } else {
                out = lerp_bounds[i]-lerp_bounds[i-1];
            }

        }

        return out;
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
