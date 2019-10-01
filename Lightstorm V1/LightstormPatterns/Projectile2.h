#ifndef Projectile2_h
#define Projectile2_h

#include "Arduino.h"

//==========================
//
//
//    Projectile 2
//
//==========================

class Projectile2 {
    
private:
    Particle *p1;
    Particle *p2;
    Particle *p3;
    Particle *p4;
    Particle *p5;
    Particle *p6;
    Particle *p7;
    Particle *p8;
public:
    int clock_count = 0;
    
    int num_objects = 2;
    float objAmt[2];
    
    int num_led = 0;
    
    void init(int n_led) {
        p1 = new Particle(0.0,0.002,0.0,-0.1);
        p2 = new Particle(0.0,0.002,0.0,-0.1);
        p3 = new Particle(0.0,0.002,0.0,-0.1);
        p4 = new Particle(0.0,0.002,0.0,-0.1);
        p5 = new Particle(0.0,0.002,0.0,-0.1);
        p6 = new Particle(0.0,0.002,0.0,-0.1);
        p7 = new Particle(0.0,0.002,0.0,-0.1);
        p8 = new Particle(0.0,0.002,0.0,-0.1);

        
        for(int i=0;i<num_objects;i++) {
            objAmt[i]=0.0;
        }
        
        num_led = n_led;
    }
    
    float get(int i) {
        float output = 0.0;
        
        float idx_to_phase  = i/float(num_led-1);
        float p1_val = 0.0;
        float p2_val = 0.0;
        float p3_val = 0.0;
        float p4_val = 0.0;
        float p5_val = 0.0;
        float p6_val = 0.0;
        float p7_val = 0.0;
        float p8_val = 0.0;
        if(p1->alive) 
            p1_val = p1->check_particle(idx_to_phase);
        if(p2->alive) 
            p2_val = p2->check_particle(idx_to_phase);
        if(p3->alive) 
            p3_val = p3->check_particle(idx_to_phase);
        if(p4->alive) 
            p4_val = p4->check_particle(idx_to_phase);
        if(p5->alive) 
            p5_val = p5->check_particle(idx_to_phase);
        if(p6->alive) 
            p6_val = p6->check_particle(idx_to_phase);
        if(p7->alive) 
            p7_val = p7->check_particle(idx_to_phase);
        if(p8->alive) 
            p8_val = p8->check_particle(idx_to_phase);
        
        output = clip(p1_val+p2_val+p3_val+p4_val+p5_val+p6_val+p7_val+p8_val,0.0,1.0);

        float obj_val = 0.0;
        float col_phase = idx_to_phase+clock_count*0.00006;
        col_phase -= floor(col_phase);
        if(col_phase < 0.5) {
            obj_val = 1.0 - col_phase*2.0;
        } else {
            obj_val = col_phase*2.0 - 1.0;
        }
        objAmt[0] = obj_val;
        objAmt[1] = 1.0 - obj_val;
        
        clock_count++;

        return output;
    }
    
    float get_obj_amt(int i) {
        return objAmt[i];
    }
    
    void update(bool trig, float lerp_speed_) {

        float speed = 0.03+(1.0-lerp_speed_)*0.07;
        int start = random(2);
        float w = 0.0;
        if(start==0) {
            w = -0.05;
        } else if(start==1) {
            w = 0.05;
            speed *= -1;
        }

        if(trig) {
            if(!p1->alive) {
                p1->fire(start, speed, -speed*(0.1-lerp_speed_*0.075), w);
            } else if(!p2->alive) {
                p2->fire(start, speed, -speed*(0.1-lerp_speed_*0.075), w);
            } else if(!p3->alive) {
                p3->fire(start, speed, -speed*(0.1-lerp_speed_*0.075), w);
            } else if(!p4->alive) {
                p4->fire(start, speed, -speed*(0.1-lerp_speed_*0.075), w);
            } else if(!p5->alive) {
                p5->fire(start, speed, -speed*(0.1-lerp_speed_*0.075), w);
            } else if(!p6->alive) {
                p6->fire(start, speed, -speed*(0.1-lerp_speed_*0.075), w);
            } else if(!p7->alive) {
                p7->fire(start, speed, -speed*(0.1-lerp_speed_*0.075), w);
            } else if(!p8->alive) {
                p8->fire(start, speed, -speed*(0.1-lerp_speed_*0.075), w);
            }
        }

        p1->update();
        p2->update();
        p3->update();
        p4->update();
        p5->update();
        p6->update();
        p7->update();
        p8->update();
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
