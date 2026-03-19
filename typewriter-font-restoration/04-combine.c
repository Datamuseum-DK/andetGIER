// cc -Wall -O0 -g 04-combine.c -o 04-combine -lm

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main(int argc, char** argv)
{
	if (argc < 3) {
		fprintf(stderr, "Usage: %s <threshold> <image> [image]...\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int width=-1,height=-1;

	float* combine_data = NULL;

	const float threshold = atof(argv[1]);
	if (threshold <= 0.0f || threshold >= 1.0f) {
		fprintf(stderr, "threshold must be between 0 and 1 (not inclusive)\n");
		exit(EXIT_FAILURE);
	}

	const int num_images = argc - 2;
	const float scalar = 1.0f / (float)(255 * num_images);

	for (int i=0; i<num_images; ++i) {
		const char* path = argv[2+i];
		int w,h;
		uint8_t* image_data = stbi_load(path, &w, &h, NULL, 1);
		if (image_data == NULL) {
			fprintf(stderr, "%s: could not open image\n", path);
			exit(EXIT_FAILURE);
		}

		if (i==0) {
			width = w;
			height = h;
			assert(combine_data == NULL);
			combine_data = calloc(width*height, sizeof *combine_data);
		} else {
			if ((w != width) || (h != height)) {
				fprintf(stderr, "all images must have the same dimensions: mismatch between %s (%d x %d) and %s (%d x %d)\n", path, w, h, argv[2], width, height);
				exit(EXIT_FAILURE);
			}
		}

		assert(combine_data != NULL);

		uint8_t* ip = image_data;
		float* cp = combine_data;
		for (int y=0; y<height; ++y) {
			for (int x=0; x<width; ++x) {
				*(cp++) += (float)(*(ip++)) * scalar;
			}
		}

		free(image_data);
	}

	uint8_t* output = malloc(width*height);
	uint8_t* op = output;
	float* cp = combine_data;
	for (int y=0; y<height; ++y) {
		for (int x=0; x<width; ++x) {
			*(op++) = *(cp++) > threshold ? 0 : 255;
		}
	}

	const char* path = "_combine.png";
	assert(stbi_write_png(path, width, height, 1, output, width));
	printf("wrote %s (%d x %d)\n", path, width, height);

	return EXIT_SUCCESS;
}
