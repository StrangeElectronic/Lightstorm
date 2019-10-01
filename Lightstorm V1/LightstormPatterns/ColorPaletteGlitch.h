#ifndef ColorPaletteGlitch_h
#define ColorPaletteGlitch_h

#include "Arduino.h"

class ColorPaletteGlitch {
	private:
		int palette[2][4][3];

	public:
		int n_col;
		int n_chan;

		int red[3] = {0,0,255};
		int pink[3] = {215,215,205};

		int orange[3] = {255,0,0};
		int simpson[3] = {0,255,255};

		int seafoam[3] = {255,255,0};
		int light_blue[3] = {255,255,205};

		int dark_blue[3] = {255,250,205};
		int off[3] = {0,255,0};

		void setup() {
			n_chan = 2;
			n_col = 4;
			for(int i=0;i<n_chan;i++)
				for(int j=0;j<n_col;j++)
					for(int k=0;k<3;k++)
						palette[i][j][k] = 0;

			for(int i=0;i<3;i++) {
				palette[0][0][i] = red[i];
				palette[1][0][i] = pink[i];

				palette[0][1][i] = orange[i];
				palette[1][1][i] = simpson[i];

				palette[0][2][i] = seafoam[i];
				palette[1][2][i] = light_blue[i];

				palette[0][3][i] = dark_blue[i];
				palette[1][3][i] = off[i];
			}
		}

		int get_r(int ch_idx, int pal_idx) {
			return palette[ch_idx][pal_idx][0];
		}

		int get_g(int ch_idx, int pal_idx) {
			return palette[ch_idx][pal_idx][1];
		}

		int get_b(int ch_idx, int pal_idx) {
			return palette[ch_idx][pal_idx][2];
		}
};

#endif