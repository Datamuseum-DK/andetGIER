// cc -Wall -O2 05a-make-atlas.c -o 05a-make-atlas -lm && ./05a-make-atlas

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

static const int num_atlas_cols = 16;
static const int num_atlas_rows = 8;

static int glyph_count;
static int glyph_width;
static int glyph_height;
static int cell_size;
static int atlas_width;
static int atlas_height;
static uint8_t* atlas_data;

static void pack(const char* name)
{
	char path[1<<10];
	snprintf(path, sizeof path, "_hi.%s.png", name);
	int w,h;
	uint8_t* image_data = stbi_load(path, &w, &h, NULL, 1);
	if (!image_data) {
		fprintf(stderr, "%s: could not open image\n", path);
		exit(EXIT_FAILURE);
	}

	if (glyph_count == 0) {
		assert(glyph_width == 0);
		assert(glyph_height == 0);
		glyph_width = w;
		glyph_height = h;
		cell_size = w*2;
		atlas_width = num_atlas_cols * cell_size;
		atlas_height = num_atlas_rows * cell_size;
		atlas_data = calloc(atlas_width*atlas_height, sizeof *atlas_data);
		printf("glyph image size: %d x %d, cell size: %d x %d, atlas size: %d x %d\n", w, h, cell_size, cell_size, atlas_width, atlas_height);
	}

	assert(w = glyph_width);
	assert(h = glyph_height);
	assert(cell_size > 0);
	assert(atlas_width > 0);
	assert(atlas_height > 0);

	const int col = glyph_count % num_atlas_cols;
	const int row = glyph_count / num_atlas_cols;

	assert((0 <= col) && (col < num_atlas_cols));
	assert((0 <= row) && (row < num_atlas_rows));

	uint8_t* rp = image_data;
	for (int y=0; y<glyph_height; ++y) {
		for (int x=0; x<glyph_width; ++x, ++rp) {
			const int ax = col * cell_size + (cell_size - glyph_width) / 2 + x;
			const int ay = row * cell_size + (cell_size - glyph_height) / 2 + y;
			if (!(0 <= ax) && (ax < atlas_width)) continue;
			if (!(0 <= ay) && (ay < atlas_height)) continue;
			atlas_data[ax + ay*atlas_width] += *rp;
		}
	}

	free(image_data);
	++glyph_count;
}

static void save_atlas(void)
{
	assert(glyph_count == 88);
	const char* path = "_raw_atlas.png";
	assert(stbi_write_png(path, atlas_width, atlas_height, 1, atlas_data, atlas_width));
	printf("wrote %s (%d x %d)\n", path, atlas_width, atlas_height);
}

int main(int argc, char** argv)
{
	pack("1");
	pack("AND"); // pack("OR"); // OR missing
	pack("2");
	pack("MUL");
	pack("3");
	pack("DIV");
	pack("4");
	pack("EQ");
	pack("5");
	pack("SEMICOLON");
	pack("6");
	pack("LBRACKET");
	pack("7");
	pack("RBRACKET");
	pack("8");
	pack("LPAREN");
	pack("9");
	pack("RPAREN");
	pack("å");
	pack("Å");
	pack("UNDERSCORE");
	pack("AND"); //pack("PIPE"); // AND has a pipe
	pack("0");
	pack("AND");
	pack("LT");
	pack("GT");
	pack("s");
	pack("S");
	pack("t");
	pack("T");
	pack("u");
	pack("U");
	pack("v");
	pack("V");
	pack("w");
	pack("W");
	pack("x");
	pack("X");
	pack("y");
	pack("Y");
	pack("z");
	pack("E"); //pack("Z"); // no Z found...
	pack("COMMA");
	pack("10");
	pack("MINUS");
	pack("PLUS");
	pack("j");
	pack("J");
	pack("k");
	pack("K");
	pack("l");
	pack("L");
	pack("m");
	pack("M");
	pack("n");
	pack("N");
	pack("o");
	pack("O");
	pack("p");
	pack("P");
	pack("q");
	pack("Q");
	pack("r");
	pack("R");
	pack("ø");
	pack("Ø");
	pack("æ");
	pack("Æ");
	pack("a");
	pack("A");
	pack("b");
	pack("B");
	pack("c");
	pack("C");
	pack("d");
	pack("D");
	pack("e");
	pack("E");
	pack("f");
	pack("F");
	pack("g");
	pack("G");
	pack("h");
	pack("H");
	pack("i");
	pack("I");
	pack("PERIOD");
	pack("COLON");

	save_atlas();

	return EXIT_SUCCESS;
}
