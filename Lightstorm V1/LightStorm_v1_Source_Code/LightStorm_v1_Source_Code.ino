#include <FastLED.h>
#include <LightstormPatterns.h>

#define brightness_pin 4
AnalogSmoother bright_smooth(analogRead(brightness_pin),3);
int clean_brightness_analog = analogRead(brightness_pin);

#define saturation_pin 3
AnalogSmoother saturation_smooth(analogRead(saturation_pin),3);
int clean_saturation_analog = analogRead(saturation_pin);
float bi_color_amt = 1.0;
float saturation_amt = 1.0;

#define palette_pin 2
AnalogSmoother palette_smooth(analogRead(palette_pin),3);
int clean_palette_analog = analogRead(palette_pin);
ColorPaletteRB colPalette1;
ColorPaletteCD colPalette2;
ColorPaletteGlitch colPalette3;
ColorPalettePrime colPalette4;

#define rate_pin 1
AnalogSmoother rate_smooth(analogRead(rate_pin),3);
int clean_rate_analog = analogRead(rate_pin);
float lerp_speed;

#define pattern_pin 0
AnalogSmoother pattern_smooth(analogRead(pattern_pin),3);
int clean_pattern_analog = analogRead(pattern_pin);
int n_pattern;
float pattern_xfade = analogRead(pattern_pin)*(float(n_pattern)/1023.0);
float pat_xfade_amt = 0.0;
float pat_xfade_buf = 0.25;

#define clock_pin 5
bool clock_high = false;
int clock_count = 0;
int trig_thresh = 500;

#define board_light_pin 8

#define button_pin 9
bool button_high = false;
int bank_state = 0;
int num_bank = 4;

#define led_pin 7
#define num_led 72
CRGB leds[num_led];

Swim PAT_1_1;
SwimInv PAT_2_1;
Spin PAT_3_1;
SpinInv PAT_4_1;
VWB PAT_5_1;

Projectile PAT_1_2;
Projectile2 PAT_2_2;
Corrupt PAT_3_2;
Pulse PAT_4_2;
VUInv PAT_5_2;

Rand PAT_1_3;
Interrupt PAT_2_3;
Steps PAT_3_3;
Ghost PAT_4_3;
VU PAT_5_3;

Simple PAT_1_4;
VUCtrl PAT_2_4;
SwimCtrl PAT_3_4;
Steps2 PAT_4_4;
Flow PAT_5_4;

int frameCount = 0;

////======================================================================
/// SETUP
//========================================================================
void setup() {
  pinMode(led_pin, OUTPUT);
  pinMode(board_light_pin,OUTPUT);
  pinMode(button_pin,INPUT);
  pinMode(13, OUTPUT);

  digitalWrite(13, LOW);

  colPalette1.setup();
  colPalette2.setup();
  colPalette3.setup();
  colPalette4.setup();

  n_pattern = 5;

  PAT_1_1.init(num_led);
  PAT_2_1.init(num_led);
  PAT_3_1.init(num_led);
  PAT_4_1.init(num_led);
  PAT_5_1.init(num_led);

  PAT_1_2.init(num_led);
  PAT_2_2.init(num_led);
  PAT_3_2.init(num_led);
  PAT_4_2.init(num_led);
  PAT_5_2.init(num_led);

  PAT_1_3.init(num_led);
  PAT_2_3.init(num_led);
  PAT_3_3.init(num_led);
  PAT_4_3.init(num_led);
  PAT_5_3.init(num_led);

  PAT_1_4.init(num_led);
  PAT_2_4.init(num_led);
  PAT_3_4.init(num_led);
  PAT_4_4.init(num_led);
  PAT_5_4.init(num_led);
  
  FastLED.addLeds<WS2812, led_pin, GRB>(leds,num_led);
  FastLED.setCorrection(TypicalLEDStrip);
}

////======================================================================
/// MAIN LOOP
//========================================================================
void loop() {

  // CHECK BUTTON
  button_update();

  // UPDATE ANALOG SMOOTHER
  clean_brightness_analog = analogRead(brightness_pin);
  clean_rate_analog = rate_smooth.update(analogRead(rate_pin));//0;//
  clean_saturation_analog = saturation_smooth.update(analogRead(saturation_pin));
  clean_palette_analog = palette_smooth.update(analogRead(palette_pin));//980;//
  clean_pattern_analog = pattern_smooth.update(analogRead(pattern_pin));

  // UPDATE PATTERN SEL
  pattern_check();

  // RATE & MODIFIER
  lerp_speed = 1.0-clip(((clean_rate_analog-rate_smooth.min_in)/float(rate_smooth.max_in)),0.0,1.0)          ;
  FastLED.setBrightness(clip(map(clean_brightness_analog,60,970,0,255),0,255));

  // UPDATE
  //****PATTERN SELECT****//
  bool trig = clock_check(analogRead(clock_pin));
  if(pattern_xfade<pat_xfade_buf) {
    if(bank_state==0) {
      PAT_1_1.update(trig,lerp_speed);
    } else if(bank_state==1) {
      PAT_1_2.update(trig,lerp_speed);
    } else if(bank_state==2) {
      PAT_1_3.update(trig,lerp_speed);
    } else if(bank_state==3) {
      PAT_1_4.update(trig,lerp_speed);
    }
    
  } else if(pattern_xfade>=pat_xfade_buf && pattern_xfade<1-pat_xfade_buf) {
    if(bank_state==0) {
      PAT_1_1.update(trig,lerp_speed);
      PAT_2_1.update(trig,lerp_speed);
    } else if(bank_state==1) {
      PAT_1_2.update(trig,lerp_speed);
      PAT_2_2.update(trig,lerp_speed);
    } else if(bank_state==2) {
      PAT_1_3.update(trig,lerp_speed);
      PAT_2_3.update(trig,lerp_speed);
    } else if(bank_state==3) {
      PAT_1_4.update(trig,lerp_speed);
      PAT_2_4.update(trig,lerp_speed);
    }
   
  } else if(pattern_xfade>=1-pat_xfade_buf && pattern_xfade<1+pat_xfade_buf) {
    if(bank_state==0) {
      PAT_2_1.update(trig,lerp_speed);
    } else if(bank_state==1) {
      PAT_2_2.update(trig,lerp_speed);
    } else if(bank_state==2) {
      PAT_2_3.update(trig,lerp_speed);
    } else if(bank_state==3) {
      PAT_2_4.update(trig,lerp_speed);
    }
    
  } else if(pattern_xfade>=1+pat_xfade_buf && pattern_xfade<2-pat_xfade_buf) {
    if(bank_state==0) {
      PAT_2_1.update(trig,lerp_speed);
      PAT_3_1.update(trig,lerp_speed);
    } else if(bank_state==1) {
      PAT_2_2.update(trig,lerp_speed);
      PAT_3_2.update(trig,lerp_speed);
    } else if(bank_state==2) {
      PAT_2_3.update(trig,lerp_speed);
      PAT_3_3.update(trig,lerp_speed);
    } else if(bank_state==3) {
      PAT_2_4.update(trig,lerp_speed);
      PAT_3_4.update(trig,lerp_speed);
    }
    
  } else if(pattern_xfade>=2-pat_xfade_buf && pattern_xfade<2+pat_xfade_buf) {
    if(bank_state==0) {
      PAT_3_1.update(trig,lerp_speed);
    } else if(bank_state==1) {
      PAT_3_2.update(trig,lerp_speed);
    } else if(bank_state==2) {
      PAT_3_3.update(trig,lerp_speed);
    } else if(bank_state==3) {
      PAT_3_4.update(trig,lerp_speed);
    }
    
  } else if(pattern_xfade>=2+pat_xfade_buf && pattern_xfade<3-pat_xfade_buf) {
    if(bank_state==0) {
      PAT_3_1.update(trig,lerp_speed);
      PAT_4_1.update(trig,lerp_speed);
    } else if(bank_state==1) {
      PAT_3_2.update(trig,lerp_speed);
      PAT_4_2.update(trig,lerp_speed);
    } else if(bank_state==2) {
      PAT_3_3.update(trig,lerp_speed);
      PAT_4_3.update(trig,lerp_speed);
    } else if(bank_state==3) {
      PAT_3_4.update(trig,lerp_speed);
      PAT_4_4.update(trig,lerp_speed);
    }
    
  } else if(pattern_xfade>=3-pat_xfade_buf && pattern_xfade<3+pat_xfade_buf) {
    if(bank_state==0) {
      PAT_4_1.update(trig,lerp_speed);
    } else if(bank_state==1) {
      PAT_4_2.update(trig,lerp_speed);
    } else if(bank_state==2) {
      PAT_4_3.update(trig,lerp_speed);
    } else if(bank_state==3) {
      PAT_4_4.update(trig,lerp_speed);
    }
    
  } else if(pattern_xfade>=3+pat_xfade_buf && pattern_xfade<4-pat_xfade_buf) {
    if(bank_state==0) {
      PAT_4_1.update(trig,lerp_speed);
      PAT_5_1.update(trig,lerp_speed);
    } else if(bank_state==1) {
      PAT_4_2.update(trig,lerp_speed);
      PAT_5_2.update(trig,lerp_speed);
    } else if(bank_state==2) {
      PAT_4_3.update(trig,lerp_speed);
      PAT_5_3.update(trig,lerp_speed);
    } else if(bank_state==3) {
      PAT_4_4.update(trig,lerp_speed);
      PAT_5_4.update(trig,lerp_speed);
    }
    
  } else if(pattern_xfade>=4-pat_xfade_buf) {
    if(bank_state==0) {
      PAT_5_1.update(trig,lerp_speed);
    } else if(bank_state==1) {
      PAT_5_2.update(trig,lerp_speed);
    } else if(bank_state==2) {
      PAT_5_3.update(trig,lerp_speed);
    } else if(bank_state==3) {
      PAT_5_4.update(trig,lerp_speed);
    }
    
  }
  
  // DRAW`
  for(int i=0; i<num_led; i++) {
    
    int col_A[3];
    col_A[0]=0;
    col_A[1]=0;
    col_A[2]=0;
    int col_B[3];
    col_B[0]=0;
    col_B[1]=0;
    col_B[2]=0;
    
     ///****PATTERN SELECT****//
    // patterns w/ cross fade
    int R = 0;
    int G = 0;
    int B = 0;
    if(pattern_xfade<pat_xfade_buf) {
      /////////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////
       /////////////////////////////////////////////////////////////
      /// 1 ///////////////////////////////////////////////////////
      float brightness = 0.0;
      if(bank_state==0) {
        brightness = PAT_1_1.get(i);
        int col[3];
        for(int obj=0;obj<PAT_1_1.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_1_1.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_1_1.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
      } else if(bank_state==1) {
        brightness = PAT_1_2.get(i);
        int col[3];
        for(int obj=0;obj<PAT_1_2.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_1_2.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_1_2.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
      } else if(bank_state==2) {
        brightness = PAT_1_3.get(i);
        int col[3];
        for(int obj=0;obj<PAT_1_3.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_1_3.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_1_3.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
      } else if(bank_state==3) {
        brightness = PAT_1_4.get(i);
        int col[3];
        for(int obj=0;obj<PAT_1_4.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_1_4.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_1_4.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
      }

      get_sat(col_A);
      
      R = col_A[0]*brightness;
      G = col_A[1]*brightness;
      B = col_A[2]*brightness;
    } else if(pattern_xfade>=pat_xfade_buf && pattern_xfade<1-pat_xfade_buf) {
      ////////////////////////////////////////////////////
      ////////////////////////////////////////////////////
      ////////////////////////////////////////////////////
       /////////////////////////////////////////////////////////////////
      /// 1 - 2 ///////////////////////////////////////////////////////
      float brightness = 0.0;
      if(bank_state==0) {
        brightness = PAT_1_1.get(i)+(PAT_2_1.get(i)-PAT_1_1.get(i))*((pat_xfade_amt-pat_xfade_buf)*(1.0/(1.0-pat_xfade_buf*2)));
        int col[3];
        for(int obj=0;obj<PAT_1_1.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_1_1.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_1_1.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
        
        for(int obj=0;obj<PAT_2_1.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_2_1.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_2_1.get_obj_amt(obj)*bi_color_amt;
          }
          col_B[0] += col[0]*scale;
          col_B[1] += col[1]*scale;
          col_B[2] += col[2]*scale;
        }
      } else if(bank_state==1) {
        brightness = PAT_1_2.get(i)+(PAT_2_2.get(i)-PAT_1_2.get(i))*((pat_xfade_amt-pat_xfade_buf)*(1.0/(1.0-pat_xfade_buf*2)));
        int col[3];
        for(int obj=0;obj<PAT_1_2.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_1_2.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_1_2.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
        
        for(int obj=0;obj<PAT_2_2.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_2_2.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_2_2.get_obj_amt(obj)*bi_color_amt;
          }
          col_B[0] += col[0]*scale;
          col_B[1] += col[1]*scale;
          col_B[2] += col[2]*scale;
        }
      } else if(bank_state==2) {
        brightness = PAT_1_3.get(i)+(PAT_2_3.get(i)-PAT_1_3.get(i))*((pat_xfade_amt-pat_xfade_buf)*(1.0/(1.0-pat_xfade_buf*2)));
        int col[3];
        for(int obj=0;obj<PAT_1_3.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_1_3.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_1_3.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
        
        for(int obj=0;obj<PAT_2_3.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_2_3.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_2_3.get_obj_amt(obj)*bi_color_amt;
          }
          col_B[0] += col[0]*scale;
          col_B[1] += col[1]*scale;
          col_B[2] += col[2]*scale;
        }
      } else if(bank_state==3) {
        brightness = PAT_1_4.get(i)+(PAT_2_4.get(i)-PAT_1_4.get(i))*((pat_xfade_amt-pat_xfade_buf)*(1.0/(1.0-pat_xfade_buf*2)));
        int col[3];
        for(int obj=0;obj<PAT_1_4.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_1_4.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_1_4.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
        
        for(int obj=0;obj<PAT_2_4.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_2_4.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_2_4.get_obj_amt(obj)*bi_color_amt;
          }
          col_B[0] += col[0]*scale;
          col_B[1] += col[1]*scale;
          col_B[2] += col[2]*scale;
        }
      }
      

      get_sat(col_A);
      get_sat(col_B);
      
      R = (col_A[0]*(1.0-pat_xfade_amt) + col_B[0]*pat_xfade_amt)*brightness;
      G = (col_A[1]*(1.0-pat_xfade_amt) + col_B[1]*pat_xfade_amt)*brightness;
      B = (col_A[2]*(1.0-pat_xfade_amt) + col_B[2]*pat_xfade_amt)*brightness;
    } else if(pattern_xfade>=1-pat_xfade_buf && pattern_xfade<1+pat_xfade_buf) {
      ////////////////////////////////////////////////////
      ////////////////////////////////////////////////////
      ////////////////////////////////////////////////////
       /////////////////////////////////////////////////////////////
      /// 2 ///////////////////////////////////////////////////////
      float brightness = 0.0;
      if(bank_state==0) {
        brightness = PAT_2_1.get(i);
        int col[3];
        for(int obj=0;obj<PAT_2_1.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_2_1.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_2_1.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
      } else if(bank_state==1) {
        brightness = PAT_2_2.get(i);
        int col[3];
        for(int obj=0;obj<PAT_2_2.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_2_2.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_2_2.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
      } else if(bank_state==2) {
        brightness = PAT_2_3.get(i);
        int col[3];
        for(int obj=0;obj<PAT_2_3.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_2_3.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_2_3.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
      } else if(bank_state==3) {
        brightness = PAT_2_4.get(i);
        int col[3];
        for(int obj=0;obj<PAT_2_4.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_2_4.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_2_4.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
      }
      

      get_sat(col_A);
      
      R = col_A[0]*brightness;
      G = col_A[1]*brightness;
      B = col_A[2]*brightness;
    } else if(pattern_xfade>=1+pat_xfade_buf && pattern_xfade<2-pat_xfade_buf) {
      ////////////////////////////////////////////////////
      ////////////////////////////////////////////////////
      ////////////////////////////////////////////////////
       /////////////////////////////////////////////////////////////////
      /// 2 - 3 ///////////////////////////////////////////////////////
      float brightness = 0.0;
      if(bank_state==0) {
        brightness = PAT_2_1.get(i)+(PAT_3_1.get(i)-PAT_2_1.get(i))*((pat_xfade_amt-pat_xfade_buf)*(1.0/(1.0-pat_xfade_buf*2)));
        int col[3];
        for(int obj=0;obj<PAT_2_1.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_2_1.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_2_1.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
        
        for(int obj=0;obj<PAT_3_1.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_3_1.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_3_1.get_obj_amt(obj)*bi_color_amt;
          }
          col_B[0] += col[0]*scale;
          col_B[1] += col[1]*scale;
          col_B[2] += col[2]*scale;
        }
      } else if(bank_state==1) {
        brightness = PAT_2_2.get(i)+(PAT_3_2.get(i)-PAT_2_2.get(i))*((pat_xfade_amt-pat_xfade_buf)*(1.0/(1.0-pat_xfade_buf*2)));
        int col[3];
        for(int obj=0;obj<PAT_2_2.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_2_2.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_2_2.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
        
        for(int obj=0;obj<PAT_3_2.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_3_2.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_3_2.get_obj_amt(obj)*bi_color_amt;
          }
          col_B[0] += col[0]*scale;
          col_B[1] += col[1]*scale;
          col_B[2] += col[2]*scale;
        }
      } else if(bank_state==2) {
        brightness = PAT_2_3.get(i)+(PAT_3_3.get(i)-PAT_2_3.get(i))*((pat_xfade_amt-pat_xfade_buf)*(1.0/(1.0-pat_xfade_buf*2)));
        int col[3];
        for(int obj=0;obj<PAT_2_3.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_2_3.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_2_3.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
        
        for(int obj=0;obj<PAT_3_3.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_3_3.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_3_3.get_obj_amt(obj)*bi_color_amt;
          }
          col_B[0] += col[0]*scale;
          col_B[1] += col[1]*scale;
          col_B[2] += col[2]*scale;
        }
      } else if(bank_state==3) {
        brightness = PAT_2_4.get(i)+(PAT_3_4.get(i)-PAT_2_4.get(i))*((pat_xfade_amt-pat_xfade_buf)*(1.0/(1.0-pat_xfade_buf*2)));
        int col[3];
        for(int obj=0;obj<PAT_2_4.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_2_4.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_2_4.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
        
        for(int obj=0;obj<PAT_3_4.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_3_4.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_3_4.get_obj_amt(obj)*bi_color_amt;
          }
          col_B[0] += col[0]*scale;
          col_B[1] += col[1]*scale;
          col_B[2] += col[2]*scale;
        }
      }
      

      get_sat(col_A);
      get_sat(col_B);
      
      R = (col_A[0]*(1.0-pat_xfade_amt) + col_B[0]*pat_xfade_amt)*brightness;
      G = (col_A[1]*(1.0-pat_xfade_amt) + col_B[1]*pat_xfade_amt)*brightness;
      B = (col_A[2]*(1.0-pat_xfade_amt) + col_B[2]*pat_xfade_amt)*brightness;
    } else if(pattern_xfade>=2-pat_xfade_buf && pattern_xfade<2+pat_xfade_buf) {
      ////////////////////////////////////////////////////
      ////////////////////////////////////////////////////
      ////////////////////////////////////////////////////
       /////////////////////////////////////////////////////////////
      /// 3 ///////////////////////////////////////////////////////
      float brightness = 0.0;
      if(bank_state==0) {
        brightness = PAT_3_1.get(i);
        int col[3];
        for(int obj=0;obj<PAT_3_1.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_3_1.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_3_1.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
      } else if(bank_state==1) {
        brightness = PAT_3_2.get(i);
        int col[3];
        for(int obj=0;obj<PAT_3_2.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_3_2.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_3_2.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
      } else if(bank_state==2) {
        brightness = PAT_3_3.get(i);
        int col[3];
        for(int obj=0;obj<PAT_3_3.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_3_3.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_3_3.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
      } else if(bank_state==3) {
        brightness = PAT_3_4.get(i);
        int col[3];
        for(int obj=0;obj<PAT_3_4.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_3_4.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_3_4.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
      }
 

      get_sat(col_A);
      
      R = col_A[0]*brightness;
      G = col_A[1]*brightness;
      B = col_A[2]*brightness;
    } else if(pattern_xfade>=2+pat_xfade_buf && pattern_xfade<3-pat_xfade_buf) {
      ////////////////////////////////////////////////////
      ////////////////////////////////////////////////////
      ////////////////////////////////////////////////////
       /////////////////////////////////////////////////////////////////
      /// 3 - 4 ///////////////////////////////////////////////////////
      float brightness = 0.0;
      if(bank_state==0) {
        brightness = PAT_3_1.get(i)+(PAT_4_1.get(i)-PAT_3_1.get(i))*((pat_xfade_amt-pat_xfade_buf)*(1.0/(1.0-pat_xfade_buf*2)));
        int col[3];
        for(int obj=0;obj<PAT_3_1.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_3_1.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_3_1.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
        
        for(int obj=0;obj<PAT_4_1.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_4_1.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_4_1.get_obj_amt(obj)*bi_color_amt;
          }
          col_B[0] += col[0]*scale;
          col_B[1] += col[1]*scale;
          col_B[2] += col[2]*scale;
        }
      } else if(bank_state==1) {
        brightness = PAT_3_2.get(i)+(PAT_4_2.get(i)-PAT_3_2.get(i))*((pat_xfade_amt-pat_xfade_buf)*(1.0/(1.0-pat_xfade_buf*2)));
        int col[3];
        for(int obj=0;obj<PAT_3_2.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_3_2.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_3_2.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
        
        for(int obj=0;obj<PAT_4_2.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_4_2.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_4_2.get_obj_amt(obj)*bi_color_amt;
          }
          col_B[0] += col[0]*scale;
          col_B[1] += col[1]*scale;
          col_B[2] += col[2]*scale;
        }
      } else if(bank_state==2) {
        brightness = PAT_3_3.get(i)+(PAT_4_3.get(i)-PAT_3_3.get(i))*((pat_xfade_amt-pat_xfade_buf)*(1.0/(1.0-pat_xfade_buf*2)));
        int col[3];
        for(int obj=0;obj<PAT_3_3.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_3_3.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_3_3.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
        
        for(int obj=0;obj<PAT_4_3.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_4_3.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_4_3.get_obj_amt(obj)*bi_color_amt;
          }
          col_B[0] += col[0]*scale;
          col_B[1] += col[1]*scale;
          col_B[2] += col[2]*scale;
        }
      } else if(bank_state==3) {
        brightness = PAT_3_4.get(i)+(PAT_4_4.get(i)-PAT_3_4.get(i))*((pat_xfade_amt-pat_xfade_buf)*(1.0/(1.0-pat_xfade_buf*2)));
        int col[3];
        for(int obj=0;obj<PAT_3_4.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_3_4.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_3_4.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
        
        for(int obj=0;obj<PAT_4_4.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_4_4.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_4_4.get_obj_amt(obj)*bi_color_amt;
          }
          col_B[0] += col[0]*scale;
          col_B[1] += col[1]*scale;
          col_B[2] += col[2]*scale;
        }
      }
      

      get_sat(col_A);
      get_sat(col_B);
      
      R = (col_A[0]*(1.0-pat_xfade_amt) + col_B[0]*pat_xfade_amt)*brightness;
      G = (col_A[1]*(1.0-pat_xfade_amt) + col_B[1]*pat_xfade_amt)*brightness;
      B = (col_A[2]*(1.0-pat_xfade_amt) + col_B[2]*pat_xfade_amt)*brightness;
    } else if(pattern_xfade>=3-pat_xfade_buf && pattern_xfade<3+pat_xfade_buf) {
      ////////////////////////////////////////////////////
      ////////////////////////////////////////////////////
      ////////////////////////////////////////////////////
       /////////////////////////////////////////////////////////////
      /// 4 ///////////////////////////////////////////////////////
      float brightness = 0.0;
      if(bank_state==0) {
        brightness = PAT_4_1.get(i);
        int col[3];
        for(int obj=0;obj<PAT_4_1.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_4_1.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_4_1.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
      } else if(bank_state==1) {
        brightness = PAT_4_2.get(i);
        int col[3];
        for(int obj=0;obj<PAT_4_2.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_4_2.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_4_2.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
      } else if(bank_state==2) {
        brightness = PAT_4_3.get(i);
        int col[3];
        for(int obj=0;obj<PAT_4_3.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_4_3.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_4_3.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
      } else if(bank_state==3) {
        brightness = PAT_4_4.get(i);
        int col[3];
        for(int obj=0;obj<PAT_4_4.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_4_4.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_4_4.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
      }
      

      get_sat(col_A);
      
      R = col_A[0]*brightness;
      G = col_A[1]*brightness;
      B = col_A[2]*brightness;
    } else if(pattern_xfade>=3+pat_xfade_buf && pattern_xfade<4-pat_xfade_buf) {
      ////////////////////////////////////////////////////
      ////////////////////////////////////////////////////
      ////////////////////////////////////////////////////
       /////////////////////////////////////////////////////////////////
      /// 4 - 5 ///////////////////////////////////////////////////////
      float brightness = 0.0;
      if(bank_state==0) {
        brightness = PAT_4_1.get(i)+(PAT_5_1.get(i)-PAT_4_1.get(i))*((pat_xfade_amt-pat_xfade_buf)*(1.0/(1.0-pat_xfade_buf*2)));
        int col[3];
        for(int obj=0;obj<PAT_4_1.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_4_1.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_4_1.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
        
        for(int obj=0;obj<PAT_5_1.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_5_1.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_5_1.get_obj_amt(obj)*bi_color_amt;
          }
          col_B[0] += col[0]*scale;
          col_B[1] += col[1]*scale;
          col_B[2] += col[2]*scale;
        }
      } else if(bank_state==1) {
        brightness = PAT_4_2.get(i)+(PAT_5_2.get(i)-PAT_4_2.get(i))*((pat_xfade_amt-pat_xfade_buf)*(1.0/(1.0-pat_xfade_buf*2)));
        int col[3];
        for(int obj=0;obj<PAT_4_2.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_4_2.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_4_2.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
        
        for(int obj=0;obj<PAT_5_2.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_5_2.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_5_2.get_obj_amt(obj)*bi_color_amt;
          }
          col_B[0] += col[0]*scale;
          col_B[1] += col[1]*scale;
          col_B[2] += col[2]*scale;
        }
      } else if(bank_state==2) {
        brightness = PAT_4_3.get(i)+(PAT_5_3.get(i)-PAT_4_3.get(i))*((pat_xfade_amt-pat_xfade_buf)*(1.0/(1.0-pat_xfade_buf*2)));
        int col[3];
        for(int obj=0;obj<PAT_4_3.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_4_3.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_4_3.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
        
        for(int obj=0;obj<PAT_5_3.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_5_3.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_5_3.get_obj_amt(obj)*bi_color_amt;
          }
          col_B[0] += col[0]*scale;
          col_B[1] += col[1]*scale;
          col_B[2] += col[2]*scale;
        }
      } else if(bank_state==3) {
        brightness = PAT_4_4.get(i)+(PAT_5_4.get(i)-PAT_4_4.get(i))*((pat_xfade_amt-pat_xfade_buf)*(1.0/(1.0-pat_xfade_buf*2)));
        int col[3];
        for(int obj=0;obj<PAT_4_4.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_4_4.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_4_4.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
        
        for(int obj=0;obj<PAT_5_4.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_5_4.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_5_4.get_obj_amt(obj)*bi_color_amt;
          }
          col_B[0] += col[0]*scale;
          col_B[1] += col[1]*scale;
          col_B[2] += col[2]*scale;
        }
      }
      

      get_sat(col_A);
      get_sat(col_B);
      
      R = (col_A[0]*(1.0-pat_xfade_amt) + col_B[0]*pat_xfade_amt)*brightness;
      G = (col_A[1]*(1.0-pat_xfade_amt) + col_B[1]*pat_xfade_amt)*brightness;
      B = (col_A[2]*(1.0-pat_xfade_amt) + col_B[2]*pat_xfade_amt)*brightness;
    } else if(pattern_xfade>=4-pat_xfade_buf && pattern_xfade<4+pat_xfade_buf) {
      ////////////////////////////////////////////////////
      ////////////////////////////////////////////////////
      ////////////////////////////////////////////////////
       /////////////////////////////////////////////////////////////
      /// 5 ///////////////////////////////////////////////////////
      float brightness = 0.0;
      if(bank_state==0) {
        brightness = PAT_5_1.get(i);
        int col[3];
        for(int obj=0;obj<PAT_5_1.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_5_1.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_5_1.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
      } else if(bank_state==1) {
        brightness = PAT_5_2.get(i);
        int col[3];
        for(int obj=0;obj<PAT_5_2.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_5_2.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_5_2.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
      } else if(bank_state==2) {
        brightness = PAT_5_3.get(i);
        int col[3];
        for(int obj=0;obj<PAT_5_3.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_5_3.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_5_3.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
      } else if(bank_state==3) {
        brightness = PAT_5_4.get(i);
        int col[3];
        for(int obj=0;obj<PAT_5_4.num_objects;obj++) {
          get_rgb(obj,col);
          float scale = 0.0;
          if(obj==0) {
            scale = 1.0*(1.0-bi_color_amt)+PAT_5_4.get_obj_amt(obj)*bi_color_amt;
          } else {
            scale = PAT_5_4.get_obj_amt(obj)*bi_color_amt;
          }
          col_A[0] += col[0]*scale;
          col_A[1] += col[1]*scale;
          col_A[2] += col[2]*scale;
        }
      }
      

      get_sat(col_A);
      
      R = col_A[0]*brightness;
      G = col_A[1]*brightness;
      B = col_A[2]*brightness;
    }

    if(R<2) {
      R=0;
    }

    leds[i] = CRGB(clip(R,0,255),clip(G,0,255),clip(B,0,255));
  }
 
  FastLED.show();
  
  // UPDATE on board LED //
 
  frameCount++;
  if(bank_state==0) {
    if(frameCount%5<2) {
      digitalWrite(board_light_pin,LOW);
    } else {
      digitalWrite(board_light_pin,HIGH);
    }
  } else if(bank_state==1) {
    if(frameCount%10<5) {
      digitalWrite(board_light_pin,LOW);
    } else {
      digitalWrite(board_light_pin,HIGH);
    }
  } else if(bank_state==2) {
    if(frameCount%20<10) {
      digitalWrite(board_light_pin,LOW);
    } else {
      digitalWrite(board_light_pin,HIGH);
    }
  } else if(bank_state==3) {
    if(frameCount%80<40) {
      digitalWrite(board_light_pin,LOW);
    } else {
      digitalWrite(board_light_pin,HIGH);
    }
  }

  delay(10);
}


////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///   COLOR FUNCTIONS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int get_hue(int obj_idx) {
  return map(clean_palette_analog,0,1023,0,255)+32*obj_idx;
}


void get_rgb(int obj_idx, int rgb[]) {
  if(bank_state == 0) {
    float col_idx = clip((clean_palette_analog-palette_smooth.min_in)*(1.0/(palette_smooth.max_in-palette_smooth.min_in))*(colPalette1.n_col-1),0,colPalette1.n_col-1);
    float xfade_amt = col_idx-floor(col_idx);
  
    float f_col_idx = floor(col_idx);
    float c_col_idx = ceil(col_idx);
  
    rgb[0] = colPalette1.get_r(obj_idx,f_col_idx)*(1.0-xfade_amt)+colPalette1.get_r(obj_idx,c_col_idx)*xfade_amt;
    rgb[1] = colPalette1.get_g(obj_idx,f_col_idx)*(1.0-xfade_amt)+colPalette1.get_g(obj_idx,c_col_idx)*xfade_amt;
    rgb[2] = colPalette1.get_b(obj_idx,f_col_idx)*(1.0-xfade_amt)+colPalette1.get_b(obj_idx,c_col_idx)*xfade_amt;
  }else if(bank_state == 1) {
    float col_idx = clip((clean_palette_analog-palette_smooth.min_in)*(1.0/(palette_smooth.max_in-palette_smooth.min_in))*(colPalette2.n_col-1),0,colPalette2.n_col-1);
    float xfade_amt = col_idx-floor(col_idx);
  
    float f_col_idx = floor(col_idx);
    float c_col_idx = ceil(col_idx);
  
    rgb[0] = colPalette2.get_r(obj_idx,f_col_idx)*(1.0-xfade_amt)+colPalette2.get_r(obj_idx,c_col_idx)*xfade_amt;
    rgb[1] = colPalette2.get_g(obj_idx,f_col_idx)*(1.0-xfade_amt)+colPalette2.get_g(obj_idx,c_col_idx)*xfade_amt;
    rgb[2] = colPalette2.get_b(obj_idx,f_col_idx)*(1.0-xfade_amt)+colPalette2.get_b(obj_idx,c_col_idx)*xfade_amt;
  }else if(bank_state == 2) {
    float col_idx = clip((clean_palette_analog-palette_smooth.min_in)*(1.0/(palette_smooth.max_in-palette_smooth.min_in))*(colPalette3.n_col-1),0,colPalette3.n_col-1);
    float xfade_amt = col_idx-floor(col_idx);
  
    float f_col_idx = floor(col_idx);
    float c_col_idx = ceil(col_idx);
  
    rgb[0] = colPalette3.get_r(obj_idx,f_col_idx)*(1.0-xfade_amt)+colPalette3.get_r(obj_idx,c_col_idx)*xfade_amt;
    rgb[1] = colPalette3.get_g(obj_idx,f_col_idx)*(1.0-xfade_amt)+colPalette3.get_g(obj_idx,c_col_idx)*xfade_amt;
    rgb[2] = colPalette3.get_b(obj_idx,f_col_idx)*(1.0-xfade_amt)+colPalette3.get_b(obj_idx,c_col_idx)*xfade_amt;
  }else if(bank_state == 3) {
    float col_idx = clip((clean_palette_analog-palette_smooth.min_in)*(1.0/(palette_smooth.max_in-palette_smooth.min_in))*(colPalette4.n_col-1),0,colPalette4.n_col-1);
    float xfade_amt = col_idx-floor(col_idx);
  
    float f_col_idx = floor(col_idx);
    float c_col_idx = ceil(col_idx);
  
    rgb[0] = colPalette4.get_r(obj_idx,f_col_idx)*(1.0-xfade_amt)+colPalette4.get_r(obj_idx,c_col_idx)*xfade_amt;
    rgb[1] = colPalette4.get_g(obj_idx,f_col_idx)*(1.0-xfade_amt)+colPalette4.get_g(obj_idx,c_col_idx)*xfade_amt;
    rgb[2] = colPalette4.get_b(obj_idx,f_col_idx)*(1.0-xfade_amt)+colPalette4.get_b(obj_idx,c_col_idx)*xfade_amt;
  }
}

void get_sat(int col[3]) {
  bi_color_amt = clip(((clean_saturation_analog-saturation_smooth.min_in)/float(saturation_smooth.max_in))*2.0-1.0,0.0,1.0);
  saturation_amt = clip(((clean_saturation_analog-saturation_smooth.min_in)/float(saturation_smooth.max_in))*2.0,0.0,1.0);

  // clac sturation to RGB given saturation amt and RGB
  int col_max = 0;
  for(int i=0; i<3; i++) {
    if(col[i]>col_max)
      col_max = col[i];
  }
  for(int i=0; i<3; i++) {
    col[i] = col[i]*saturation_amt+col_max*(1.0-saturation_amt);
  }
  
}


////+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/// INTERFACING
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void pattern_check() {
  pattern_xfade = clip((clean_pattern_analog-pattern_smooth.min_in)*(float(n_pattern-1)/float(pattern_smooth.max_in-pattern_smooth.min_in)),0,n_pattern-1);
  pat_xfade_amt = clip((pattern_xfade-int(pattern_xfade))*(1+pat_xfade_buf*2)-pat_xfade_buf,0.0,1.0);
}

bool clock_check(int clock_val) {
  bool check = false;
  if(clock_val>=trig_thresh && clock_high==false) {
    check = true;
    clock_count++;
    clock_high = true;
  }
  if(clock_val<trig_thresh && clock_high==true) {
    clock_high = false;
  }
  
  return check;
}

bool button_check(int button_val) {
  bool check = false;
  if(button_val==HIGH && button_high==false) {
    check = true;
    button_high = true;
  }
  if(button_val==LOW && button_high==true) {
    button_high = false;
  }
  
  return check;
}

void button_update() {
  bool button_trig = button_check(digitalRead(button_pin));

  if(button_trig) {
    bank_state++;
    bank_state = bank_state%num_bank;
  }
}

////======================================================================
/// GENERAL FUNCTIONS
//========================================================================
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
