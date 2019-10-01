#ifndef ColorPalettePrime_h
#define ColorPalettePrime_h

#include "Arduino.h"

class ColorPalettePrime {
private:
	int palette[2][4][3];

public:
	// Colors

	int n_col;
	int darken = 0;

	int lime[3];
	int seafoam[3];
	int lavender[3];
	int pink[3];

	int yellow[3];
	int purple[3];
	int ice[3];
	int peach[3];

	void setup() {
		n_col = 4;

		lime[0] = 255;
		lime[1] = 0;
		lime[2] = 0;
		palette[0][0][0] = lime[0];
		palette[0][0][1] = lime[1];
		palette[0][0][2] = lime[2];

		seafoam[0] = 190;
		seafoam[1] = 50;
		seafoam[2] = 205;
		palette[1][0][0] = seafoam[0];
		palette[1][0][1] = seafoam[1];
		palette[1][0][2] = seafoam[2];


		lavender[0] = 255;
		lavender[1] = 225;
		lavender[2] = 0;
		palette[0][1][0] = lavender[0];
		palette[0][1][1] = lavender[1];
		palette[0][1][2] = lavender[2];

		pink[0] = 255;
		pink[1] = 185;
		pink[2] = 0;
		palette[1][1][0] = pink[0];
		palette[1][1][1] = pink[1];
		palette[1][1][2] = pink[2];


		yellow[0] = 0;
		yellow[1] = 255;
		yellow[2] = 105;
		palette[0][2][0] = yellow[0];
		palette[0][2][1] = yellow[1];
		palette[0][2][2] = yellow[2];

		purple[0] = 55;
		purple[1] = 255;
		purple[2] = 0;
		palette[1][2][0] = purple[0];
		palette[1][2][1] = purple[1];
		palette[1][2][2] = purple[2];


		ice[0] = 0;
		ice[1] = 255;
		ice[2] = 0;
		palette[0][3][0] = ice[0];
		palette[0][3][1] = ice[1];
		palette[0][3][2] = ice[2];

		peach[0] = 50;
		peach[1] = 255;
		peach[2] = 0;
		palette[1][3][0] = peach[0];
		palette[1][3][1] = peach[1];
		palette[1][3][2] = peach[2];
	}

	int get_r(int ch_idx, int pal_idx) {
		int out = palette[ch_idx][pal_idx][0];
		out = out - darken*(1.0-out/255.0);
        if(out<0)
            out=0;
		return out;
	}

	int get_g(int ch_idx, int pal_idx) {
		int out = palette[ch_idx][pal_idx][1];
		out = out - darken*(1.0-out/255.0);
        if(out<0)
            out=0;
		return out;
	}

	int get_b(int ch_idx, int pal_idx) {
		int out = palette[ch_idx][pal_idx][2];
		out = out - darken*(1.0-out/255.0);
        if(out<0)
            out=0;
		return out;
	}

};

#endif
