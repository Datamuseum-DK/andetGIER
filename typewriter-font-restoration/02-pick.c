// cc -I/usr/local/include -Wall -O0 -g 02-pick.c -o 02-pick -L/usr/local/lib -lm -lSDL3

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <SDL3/SDL.h>

__attribute__ ((noreturn))
static void sdl_fatal_error(void)
{
	fprintf(stderr, "SDL3 fatal error: %s\n", SDL_GetError());
	exit(EXIT_FAILURE);
}

static double pan_x = 0.0;
static double pan_y = 0.0;
static double scale = 1.0;
static int window_width, window_height;

static void map_screen_to_local(double sx, double sy, double* out_lx, double* out_ly)
{
	const double mid_x = (double)window_width * 0.5;
	const double mid_y = (double)window_height * 0.5;
	double lx = (sx - mid_x - pan_x) / scale;
	double ly = (sy - mid_y - pan_y) / scale;
	if (out_lx) *out_lx = lx;
	if (out_ly) *out_ly = ly;
}


struct metrics {
	float left;
	float right;
	float top;
	float bottom;
	float x_margin;
	float top_margin;
	float bottom_margin;
};

int main(int argc, char** argv)
{
	if (argc != 10) {
		fprintf(stderr, "Usage: %s <left> <right> <top> <bottom> <x-margin> <top-margin> <bottom-margin> <image-path> <pick-path>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	struct metrics metrics;
	metrics.left          = atoi(argv[1]);
	metrics.right         = atoi(argv[2]);
	metrics.top           = atoi(argv[3]);
	metrics.bottom        = atoi(argv[4]);
	metrics.x_margin      = atoi(argv[5]);
	metrics.top_margin    = atoi(argv[6]);
	metrics.bottom_margin = atoi(argv[7]);

	if (metrics.x_margin < 0) {
		fprintf(stderr, "<x-margin> cannot be negative\n");
		exit(EXIT_FAILURE);
	}

	if (metrics.top_margin < 0) {
		fprintf(stderr, "<top-margin> cannot be negative\n");
		exit(EXIT_FAILURE);
	}

	if (metrics.bottom_margin < 0) {
		fprintf(stderr, "<bottom-margin> cannot be negative\n");
		exit(EXIT_FAILURE);
	}

	if (metrics.left > metrics.right) {
		fprintf(stderr, "<left> cannot be greater than <right>\n");
		exit(EXIT_FAILURE);
	}

	if (metrics.top > metrics.bottom) {
		fprintf(stderr, "<top> cannot be greater than <bottom>\n");
		exit(EXIT_FAILURE);
	}

	int image_width, image_height;
	uint8_t* image_data = stbi_load(argv[8], &image_width, &image_height, NULL, 3);
	if (image_data == NULL) {
		fprintf(stderr, "%s: could not open image\n", argv[8]);
		exit(EXIT_FAILURE);
	}

	if (!SDL_Init(SDL_INIT_VIDEO)) sdl_fatal_error();

	SDL_Window* window = SDL_CreateWindow(
		"02-pick",
		1024, 1024,
		SDL_WINDOW_RESIZABLE);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
	if (renderer == NULL) sdl_fatal_error();

	const Uint32 desired_format = SDL_PIXELFORMAT_RGB24;
	const int desired_access = SDL_TEXTUREACCESS_STATIC;
	SDL_Texture* texture = SDL_CreateTexture(renderer, desired_format, desired_access, image_width, image_height);
	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
	SDL_UpdateTexture(texture, NULL, image_data, 3*image_width);

	int is_exiting = 0;
	int is_panning = 0;
	int mouse_x = 0;
	int mouse_y = 0;
	while (!is_exiting) {
		int do_clip = 0;
		SDL_GetWindowSize(window, &window_width, &window_height);
		SDL_Event ev;
		while (SDL_PollEvent(&ev)) {
			if ((ev.type == SDL_EVENT_QUIT) || (ev.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)) {
				is_exiting = 1;
			} else if (ev.type == SDL_EVENT_KEY_DOWN) {
				if (ev.key.scancode == SDL_SCANCODE_ESCAPE) is_exiting = 1;
			} else if (ev.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
				if (ev.button.button == SDL_BUTTON_LEFT) {
					do_clip = 1;
				} else if (ev.button.button == SDL_BUTTON_RIGHT) {
					is_panning = 1;
				}
			} else if (ev.type == SDL_EVENT_MOUSE_BUTTON_UP) {
				if (ev.button.button == SDL_BUTTON_RIGHT) {
					is_panning = 0;
				}
			} else if (ev.type == SDL_EVENT_MOUSE_MOTION) {
				mouse_x = ev.motion.x;
				mouse_y = ev.motion.y;
				if (is_panning) {
					pan_x += (double)ev.motion.xrel;
					pan_y += (double)ev.motion.yrel;
				}
			} else if (ev.type == SDL_EVENT_MOUSE_WHEEL) {
				const double mx = ev.wheel.mouse_x;
				const double my = ev.wheel.mouse_y;
				double plx,ply;
				map_screen_to_local(mx, my, &plx, &ply);
				scale *= pow(1.017, ev.wheel.y);
				double lx,ly;
				map_screen_to_local(mx, my, &lx, &ly);
				pan_x += (lx-plx)*scale;
				pan_y += (ly-ply)*scale;
			}
		}

		if (do_clip) {
			double mx,my;
			map_screen_to_local(mouse_x, mouse_y, &mx, &my);
			mx += image_width/2;
			my += image_height/2;
			const int x0 = mx + (metrics.left - metrics.x_margin);
			const int y0 = my + (metrics.top - metrics.top_margin);
			const int x1 = mx + (metrics.right + metrics.x_margin);
			const int y1 = my + (metrics.bottom + metrics.bottom_margin);
			const int w = x1-x0;
			const int h = y1-y0;
			uint8_t* data = malloc(w*h);
			uint8_t* p = data;
			for (int y=y0; y<y1; ++y) {
				for (int x=x0; x<x1; ++x) {
					*(p++) = image_data[x*3 + y*3*image_width];
				}
			}
			const char* name = argv[9];
			assert(stbi_write_png(name, w, h, 1, data, w));
			printf("wrote %s (%dx%d)\n", name, w, h);
			is_exiting = 1;
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_FRect dst;
		{
			const int ex = (double)image_width*0.5*scale;
			const int ey = (double)image_height*0.5*scale;
			const int mid_x = (window_width >> 1) + pan_x;
			const int mid_y = (window_height >> 1) + pan_y;
			dst.x = mid_x-ex;
			dst.y = mid_y-ey;
			dst.w = ex*2;
			dst.h = ey*2;
		}
		SDL_RenderTexture(renderer, texture, NULL, &dst);

		{
			SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
			const float x0 = mouse_x + metrics.left * scale;
			const float y0 = mouse_y + metrics.top * scale;
			const float x1 = mouse_x + metrics.right * scale;
			const float y1 = mouse_y + metrics.bottom * scale;
			SDL_RenderLine(renderer, x0, y0, x1, y0);
			SDL_RenderLine(renderer, x1, y0, x1, y1);
			SDL_RenderLine(renderer, x1, y1, x0, y1);
			SDL_RenderLine(renderer, x0, y1, x0, y0);
		}

		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
			const float x0 = mouse_x + (metrics.left - metrics.x_margin) * scale;
			const float y0 = mouse_y + (metrics.top - metrics.top_margin) * scale;
			const float x1 = mouse_x + (metrics.right + metrics.x_margin) * scale;
			const float y1 = mouse_y + (metrics.bottom + metrics.bottom_margin) * scale;
			SDL_RenderLine(renderer, x0, y0, x1, y0);
			SDL_RenderLine(renderer, x1, y0, x1, y1);
			SDL_RenderLine(renderer, x1, y1, x0, y1);
			SDL_RenderLine(renderer, x0, y1, x0, y0);
		}

		SDL_RenderPresent(renderer);
	}

	return EXIT_SUCCESS;
}
