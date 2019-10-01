// #ifndef ColorPaletteCD_h
#define ColorPaletteCD_h

#include "Arduino.h"

class ColorPaletteCD {
	private:
		int palette[2][4][3];

	public:
		int n_col;
		int n_chan;

		int col_0_0[3] = {251,187,0};
		int col_1_0[3] = {105,140,10};

		int col_0_1[3] = {200,124,11};
		int col_1_1[3] = {60,205,0};

		int col_0_2[3] = {24,235,4};
		int col_1_2[3] = {118,208,8};

		int col_0_3[3] = {145,190,200};
		int col_1_3[3] = {176,65,233};

		void setup() {
			n_chan = 2;
			n_col = 4;
			for(int i=0;i<n_chan;i++)
				for(int j=0;j<n_col;j++)
					for(int k=0;k<3;k++)
						palette[i][j][k] = 0;

			for(int i=0;i<3;i++) {
				palette[0][0][i] = col_0_0[i];
				palette[1][0][i] = col_1_0[i];

				palette[0][1][i] = col_0_1[i];
				palette[1][1][i] = col_1_1[i];

				palette[0][2][i] = col_0_2[i];
				palette[1][2][i] = col_1_2[i];

				palette[0][3][i] = col_0_3[i];
				palette[1][3][i] = col_1_3[i];
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