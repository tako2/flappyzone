#include "flappyzone.h"

#include "font_fzone.h"

//=============================================================================
#include "ch_opaopa.h"
#include "ch_opaopa_wing.h"

const Bitmap_t bmp_opaopa[] = {
	{ 5, 2, ch_opaopa_array },
	{ 5, 2, ch_opaopa_array + 5 * 2 },
	{ 5, 2, ch_opaopa_array + 5 * 4 },
	{ 5, 3, ch_opaopa_array + 5 * 6 },
};

const Bitmap_t bmp_opaopa_wing[] = {
	{ 1, 2, ch_opaopa_wing_array },
	{ 1, 2, ch_opaopa_wing_array + 2 },
	{ 1, 2, ch_opaopa_wing_array + 4 },
	{ 1, 2, ch_opaopa_wing_array + 6 },

	{ 1, 2, ch_opaopa_wing_array + 8 },
	{ 1, 2, ch_opaopa_wing_array + 10 },
	{ 1, 2, ch_opaopa_wing_array + 12 },
	{ 1, 2, ch_opaopa_wing_array + 14 },

	{ 1, 2, ch_opaopa_wing_array + 16 },
	{ 1, 2, ch_opaopa_wing_array + 18 },
	{ 1, 2, ch_opaopa_wing_array + 20 },
	{ 1, 2, ch_opaopa_wing_array + 22 },

	{ 1, 2, ch_opaopa_wing_array + 24 },
	{ 1, 2, ch_opaopa_wing_array + 26 },
	{ 1, 2, ch_opaopa_wing_array + 28 },
	{ 1, 2, ch_opaopa_wing_array + 30 },
};

//=============================================================================
#include "ch_particle.h"

const Bitmap2_t bmp_particle[] = {
	{
		{ 2, 2, ch_particle1_array },
		{ 2, 2, ch_particle1_array + 2 * 2 },
	},
	{
		{ 3, 2, ch_particle2_array },
		{ 3, 2, ch_particle2_array + 3 * 2 },
	}
};

//=============================================================================
#define OPAOPA_X 30

const uint8_t opop_pat[] = { 3, 3, 2, 1, 0, 0, 0, 2 };

//=============================================================================
void draw_opaopa(/*uint8_t x, */ int16_t y, uint8_t anim_cnt)
{
	put_bitmap(OPAOPA_X+1, y >> 3, &bmp_opaopa[(y>>1) & 3], 0);
	put_bitmap(OPAOPA_X, y >> 3,
			   &bmp_opaopa_wing[((y>>1) & 3) * 4 + (opop_pat[anim_cnt & 7])],
			   WHITE);
	if ((y & 6) == 6) {
		insert_attr((y >> 3), OPAOPA_X+1, 2, RED);
		insert_attr((y >> 3) + 1, OPAOPA_X+1, 2, RED);
		insert_attr((y >> 3) + 2, OPAOPA_X+1, 2, RED);
		insert_attr((y >> 3), OPAOPA_X+3, 3, CYAN);
		insert_attr((y >> 3) + 1, OPAOPA_X+3, 3, CYAN);
	} else {
		insert_attr((y >> 3), OPAOPA_X+1, 2, RED);
		insert_attr((y >> 3) + 1, OPAOPA_X+1, 2, RED);
		insert_attr((y >> 3), OPAOPA_X+3, 3, CYAN);
	}
}

//=============================================================================
typedef struct {
	int8_t x;
	int8_t y;
	int8_t space;
} Column_t;
#define COLUMN_W 8

//=============================================================================
uint8_t appear_column(Column_t *col)
{
	if (col->space == 0) {
		col->x = 80;
		col->y = 5 + (rand() & 7);
		col->space = 6;
		return (1);
	}
	return (0);
}

//=============================================================================
uint8_t update_column(Column_t *col)
{
	col->x --;
	if (col->x < -8) {
		col->space = 0;
	}
	if (col->x == OPAOPA_X) {
		return (1);
	}
	return (0);
}

//=============================================================================
void draw_column(Column_t *col)
{
	fill_rect(col->x,          0, COLUMN_W,   col->y-2, 0xff, 0);
	fill_rect(col->x-1, col->y-2, COLUMN_W+2,        2, 0xff, 0);

	fill_rect(col->x-1, col->y+col->space, COLUMN_W+2, 2, 0xff, 0);
	fill_rect(col->x,   col->y+col->space+2, COLUMN_W, 22-(col->y+col->space), 0xff, 0);
}

//=============================================================================
typedef struct {
	uint16_t x;
	uint16_t y;
} Particle_t;

const int8_t par_dx[] = { 3, 2, 2, 1, 0, -1, -2, -2,
						   -3, -2, -2, -1,  0,  1,  2,  2 };
const int8_t par_dy[] = { 0, 1, 2, 2, 3,  2,  2,  1,
						    0, -1, -2, -2, -3, -2, -2, -1 };
Particle_t particle[48];
uint8_t part_cnt;

//=============================================================================
void set_particle(uint16_t x, uint16_t y)
{
	uint8_t cnt;
	Particle_t *par;
	par = particle;
	x ++;
	x <<= 2;
	y <<= 2;
	for (cnt = 0; cnt < 48; cnt ++) {
		if (cnt < 16) {
			par->x = x;
			par->y = y;
		} else if (cnt < 32) {
			par->x = x + par_dx[cnt - 16];
			par->y = y + par_dy[cnt - 16];
		} else {
			par->x = x + par_dx[cnt - 32];
			par->y = y + par_dy[cnt - 32];
		}
		par ++;
	}
	part_cnt = 0;
}

//=============================================================================
void draw_particle()
{
	uint8_t cnt;

	if (part_cnt > 20) {
		return;
	}

	Particle_t *par;
	par = particle;
	for (cnt = 48; cnt > 0; cnt --) {
		put_bitmap2(par->x >> 2, par->y >> 2,
					(void *)&bmp_particle[part_cnt & 1], CYAN);
		par ++;
	}
}

//=============================================================================
uint8_t update_particle()
{
	uint8_t cnt;

	if (part_cnt > 20) {
		return (0);
	}

	Particle_t *par;
	par = particle;
	for (cnt = 0; cnt < 48; cnt ++) {
		if (part_cnt < 5) {
			if (cnt < 16) {
				par->x += par_dx[cnt & 15];
				par->y += par_dy[cnt & 15];
			} else if (cnt < 32) {
				par->x += par_dx[cnt & 15]*2;
				par->y += par_dy[cnt & 15]*2;
			} else {
				par->x += par_dx[cnt & 15]*3;
				par->y += par_dy[cnt & 15]*3;
			}
		} else {
			if (cnt < 16) {
				par->x += par_dx[cnt & 15] /2;
				par->y += par_dy[cnt & 15] /2;
			} else if (cnt < 32) {
				par->x += par_dx[cnt & 15];
				par->y += par_dy[cnt & 15];
			} else {
				par->x += par_dx[cnt & 15]*2;
				par->y += par_dy[cnt & 15]*2;
			}
		}
		par ++;
	}
	part_cnt ++;

	return (1);
}

//=============================================================================
static uint8_t score_str[3];
void draw_score(uint8_t y, uint8_t score)
{
	bcd_to_ascii(&score, 1, score_str);

	if (score < 10) {
		fill_rect(40-3, y, 7, 2, 0x00, 0);
		font_put(40-1, y, score_str[1], WHITE);
	} else {
		fill_rect(40-5, y, 10, 2, 0x00, 0);
		font_puts(40-3, y, score_str, WHITE);		
	}
}

//=============================================================================
uint8_t game_state;
#define GAME_ST_START 0
#define GAME_ST_IN_GAME 1
#define GAME_ST_FAILED1 2
#define GAME_ST_FAILED2 3
#define GAME_ST_GAMEOVER 4
uint8_t state_cnt;

//=============================================================================
void main()
{
	init_screen();
	font_init(font_fzone_array);

	int16_t y, vy;
	uint8_t anim_cnt;

	y = 10 << 3;
	vy = 0;
	anim_cnt = 0;

	const int8_t dy[] = { -2, -2, 0, 0, 1, 1, 1, 1 };

	Column_t col[4];
	uint8_t num_cols;
	uint8_t cnt;

	for (cnt = 0; cnt < 4; cnt ++) {
		col[cnt].x = 80;
		col[cnt].y = 0;
		col[cnt].space = 0;
	}

	game_state = GAME_ST_START;

	uint8_t key;

	uint8_t score, hiscore;

	score = 0;
	hiscore = 0;

	while (1) {
		key = in_funckey();

		draw_hlines(0, 24, 0);
		if ((anim_cnt & 1) == 0) {
			draw_hlines(24, 1, 0x2244);
		} else {
			draw_hlines(24, 1, 0x4422);
		}
		clear_attr(GREEN);
		insert_attr(24, 0, 80, YELLOW);

		switch (game_state) {
		case GAME_ST_START:
			font_puts(23, 4, "FLAPPY ZONE", YELLOW);
			font_puts(16, 19, "HIT [SHIFT] KEY !", WHITE);
			font_puts(40-8, 22, "V1.03", CYAN);
			break;
		case GAME_ST_IN_GAME:
			num_cols = 0;
			for (cnt = 0; cnt < 4; cnt ++) {
				if (col[cnt].space > 0) {
					num_cols ++;
					draw_column(&col[cnt]);
					if (update_column(&col[cnt]) != 0) {
						bcd_add8(&score, 1, 1);
					}
					if ((col[cnt].x - OPAOPA_X) <= 5
						&& (col[cnt].x - OPAOPA_X) >= -COLUMN_W) {
						if (y < (col[cnt].y<<3)
							|| (y+12) > ((col[cnt].y+col[cnt].space)<<3)) {
							game_state = GAME_ST_FAILED1;
							set_particle(OPAOPA_X, y >> 2);
							state_cnt = 0;
						}
					}
				}
			}
			if (num_cols < 2) {
				waitVBlank();
			}
			draw_score(2, score);
			break;
		case GAME_ST_FAILED2:
		case GAME_ST_FAILED1:
		case GAME_ST_GAMEOVER:
			for (cnt = 0; cnt < 4; cnt ++) {
				if (col[cnt].space > 0) {
					draw_column(&col[cnt]);
				}
			}
			break;
		}

		if (game_state != GAME_ST_GAMEOVER && game_state != GAME_ST_FAILED2) {
			draw_opaopa(y, anim_cnt);
		}

		switch (game_state) {
		case GAME_ST_START:
			y += dy[anim_cnt & 7];

			if ((key & KEY_SHIFT) == 0) {
				game_state = GAME_ST_IN_GAME;
				score = 0;
				y = 10 << 3;
				vy = -6;

				for (cnt = 0; cnt < 4; cnt ++) {
					col[cnt].x = 80;
					col[cnt].y = 0;
					col[cnt].space = 0;
				}

				state_cnt = 0;
				appear_column(&col[0]);
			}
			break;
		case GAME_ST_IN_GAME:
			state_cnt ++;
			if (state_cnt >= 40) {
				state_cnt = 0;
				for (cnt = 0; cnt < 4; cnt ++) {
					if (col[cnt].space == 0) {
						appear_column(&col[cnt]);
						break;
					}
				}
			}
			if (vy < 6) {
				vy ++;
			}
			if ((key & KEY_SHIFT) == 0) {
				vy = -6;
			}
			y += vy;
			if (y >= (((22 << 2) + 2) << 1)) {
				game_state = GAME_ST_FAILED1;
				set_particle(OPAOPA_X, y >> 2);
				state_cnt = 0;
			}
			waitVBlank();
			break;
		case GAME_ST_FAILED1:
			if (state_cnt > 20) {
				beep(0xa0, 0xa0, 20);
				game_state = GAME_ST_FAILED2;
			} else {
				state_cnt ++;
			}
			break;
		case GAME_ST_FAILED2:
			draw_particle();
			if (update_particle() == 0) {
				game_state = GAME_ST_GAMEOVER;
				if (score > hiscore) {
					hiscore = score;
				}
			}
			break;
		case GAME_ST_GAMEOVER:
			font_puts(40-10, 4, " SCORE ", CYAN);
			draw_score(7, score);
			font_puts(40-9, 11, " BEST ", RED);
			font_puts(16, 19, "HIT [SHIFT] KEY !", WHITE);
			draw_score(14, hiscore);
			if ((key & KEY_SHIFT) == 0) {
				game_state = GAME_ST_START;
				y = 10 << 3;
				vy = 0;
			}
			break;
		}

		if (game_state != GAME_ST_GAMEOVER
			&& game_state != GAME_ST_FAILED1
			&& game_state != GAME_ST_FAILED2) {
			anim_cnt ++;
		}

		render_attr();

		swap_screen();
	}
}
