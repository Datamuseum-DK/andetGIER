// cc -Wall -O3 03-correlate.c -o 03-correlate -lm
// cc -Wall -O0 -g 03-correlate.c -o 03-correlate -lm

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

static void invert(uint8_t* im, int w, int h)
{
	const int n=w*h;
	uint8_t* p = im;
	for (int i=0; i<n; ++i, ++p) {
		*p = 255-(*p);
	}
}

static const char* basename(const char* path)
{
	const char* x = path;
	for (const char* p = path; *p; ++p) if (*p == '/') x=p+1;
	return x;
}

int main(int argc, char** argv)
{
	if (argc != 4) {
		fprintf(stderr, "Usage: %s <_pick.png> <image.png> <num extracts>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int pick_width, pick_height;
	uint8_t* pick_data = stbi_load(argv[1], &pick_width, &pick_height, NULL, 1);
	if (pick_data == NULL) {
		fprintf(stderr, "%s: could not open image\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	invert(pick_data, pick_width, pick_height);

	int image_width, image_height;
	uint8_t* image_data = stbi_load(argv[2], &image_width, &image_height, NULL, 1);
	if (image_data == NULL) {
		fprintf(stderr, "%s: could not open image\n", argv[2]);
		exit(EXIT_FAILURE);
	}
	invert(image_data, image_width, image_height);

	const int num_extracts = atoi(argv[3]);
	if (num_extracts <= 0) {
		fprintf(stderr, "\"%s\" is not a valid number of extracts\n", argv[3]);
		exit(EXIT_FAILURE);
	}

	const int cw = image_width  - pick_width;
	const int ch = image_height - pick_height;

	int* cs = calloc(cw*ch, sizeof *cs);

	uint8_t* a0 = image_data;
	int* csp = cs;
	for (int y0=0; y0<ch; ++y0) {
		for (int x0=0; x0<cw; ++x0) {
			uint8_t* a1 = a0;
			uint8_t* b1 = pick_data;
			int score = 0;
			for (int y1=0; y1<pick_height; ++y1) {
				for (int x1=0; x1<pick_width; ++x1) {
					score += (*a1) * (*b1);
					++a1;
					++b1;
				}
				a1 += cw;
			}
			++a0;
			*(csp++) = score;
		}
		a0 += pick_width;
	}

	invert(image_data, image_width, image_height);

	uint8_t* extract_data = malloc(pick_width*pick_height);

	int first_best_score = -1;
	for (int i=0; i<num_extracts; ++i) {
		int best_score = -1;
		int best_x = -1;
		int best_y = -1;
		csp = cs;
		for (int y=0; y<ch; ++y) {
			for (int x=0; x<cw; ++x) {
				const int score = *(csp++);
				if (score > best_score) {
					best_score = score;
					best_x = x;
					best_y = y;
				}
			}
		}

		assert(best_score >= 0);
		assert(best_x >= 0);
		assert(best_y >= 0);

		if (i==0) first_best_score = best_score;

		char path[1<<10];
		snprintf(path, sizeof path, "_correlate.%s.%.9d.png", basename(argv[2]), first_best_score - best_score);

		printf("score %d at %d x %d => %s\n", best_score, best_x, best_y, path);

		uint8_t* xp = extract_data;
		for (int py=0; py<pick_height; ++py) {
			for (int px=0; px<pick_width; ++px) {
				const int x = best_x+px;
				const int y = best_y+py;
				assert((0 <= x) && (x < image_width));
				assert((0 <= y) && (y < image_height));
				*(xp++) = image_data[x + y * image_width];
			}
		}

		assert(stbi_write_png(path, pick_width, pick_height, 1, extract_data, pick_width));

		// blank out scores around best candidate to prevent re-picking it
		const int pw = pick_width/4;
		const int ph = pick_height/4;
		for (int dy=-ph; dy<ph; ++dy) {
			for (int dx=-pw; dx<pw; ++dx) {
				const int x = best_x+dx;
				const int y = best_y+dy;
				if (!(0 <= x && x < cw)) continue;
				if (!(0 <= y && y < ch)) continue;
				cs[x + y*cw] = 0;
			}
		}
	}

	return EXIT_SUCCESS;
}
