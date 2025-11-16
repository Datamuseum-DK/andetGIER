#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include <GL/gl.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengles2.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define ARRAY_LENGTH(xs) (sizeof(xs)/sizeof((xs)[0]))

static void _glcheck(const char* file, const int line, const char* body)
{
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		if (body == NULL) {
			fprintf(stderr, "GL ERROR 0x%.4x in %s:%d\n", err, file, line);
		} else {
			fprintf(stderr, "GL ERROR 0x%.4x in %s:%d in code: `%s`\n", err, file, line, body);
		}
		abort();
	}
}
#define GLCHECK      _glcheck(__FILE__, __LINE__, NULL)
#define GLCALL(BODY)  { BODY; _glcheck(__FILE__, __LINE__, #BODY); }

struct texture {
	GLuint gl_texture;
	int width, height;
};

static void texture_load(struct texture* tex, const char* path)
{
	memset(tex, 0, sizeof *tex);
	int n=-1;
	uint8_t* data = stbi_load(path, &tex->width, &tex->height, &n, 1);
	assert((data != NULL) && "could not open texture");
	printf("%d\n", n);
	assert(n==1);

	GLCALL(glGenTextures(1, &tex->gl_texture));
	GLCALL(glBindTexture(GL_TEXTURE_2D, tex->gl_texture));

	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	//int internal_format = GL_RGBA;
	//GLenum gl_format = GL_RGBA;
	int internal_format = GL_LUMINANCE;
	GLenum gl_format = GL_LUMINANCE;

	GLCALL(glTexImage2D(
		GL_TEXTURE_2D,
		0/*=level*/,
		internal_format,
		tex->width,
		tex->height,
		0,
		gl_format,
		GL_UNSIGNED_BYTE,
		data));

	GLCALL(glGenerateMipmap(GL_TEXTURE_2D));

	free(data);
}

static GLuint create_shader(GLenum type, const char* src)
{
	GLuint shader = glCreateShader(type); GLCHECK;

	GLCALL(glShaderSource(shader, 1, &src, NULL));
	GLCALL(glCompileShader(shader));

	GLint status;
	GLCALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &status));
	if (status == GL_FALSE) {
		GLint msglen;
		GLCALL(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &msglen));
		GLchar* msg = (GLchar*) malloc(msglen + 1);
		assert(msg != NULL);
		GLCALL(glGetShaderInfoLog(shader, msglen, NULL, msg));
		const char* stype = type == GL_VERTEX_SHADER ? "VERTEX" : type == GL_FRAGMENT_SHADER ? "FRAGMENT" : "???";

		// attempt to parse "0:<linenumber>" in error message
		int line_number = 0;
		if (strlen(msg) >= 3 && msg[0] == '0' && msg[1] == ':' && isdigit(msg[2])) {
			const char* p0 = msg+2;
			const char* p1 = p0+1;
			while (isdigit(*p1)) p1++;
			char buf[32];
			const int n = p1-p0;
			if (n < ARRAY_LENGTH(buf)) {
				memcpy(buf, p0, n);
				buf[n] = 0;
				line_number = atoi(buf);
			}
		}

		fprintf(stderr, "%s GLSL COMPILE ERROR: %s in:\n", stype, msg);
		if (line_number > 0) {
			const char* p = src;
			int remaining_line_breaks_to_find = line_number;
			while (remaining_line_breaks_to_find > 0) {
				for (;;) {
					char ch = *p;
					if (ch == 0) {
						remaining_line_breaks_to_find = 0;
						break;
					} else if (ch == '\n') {
						remaining_line_breaks_to_find--;
						p++;
						break;
					}
					p++;
				}
			}
			fwrite(src, p-src, 1, stderr);
			printf("~^~^~^~ ERROR ~^~^~^~\n");
			printf("%s\n", p);
		} else {
			fprintf(stderr, "%s\n", src);
		}

		abort();
	}

	return shader;
}

struct glyph_vertex {
	uint16_t x,y,u,v;
};

static GLuint create_render_program(const char* vert_src, const char* frag_src)
{
	GLuint vs = create_shader(GL_VERTEX_SHADER, vert_src);
	GLuint fs = create_shader(GL_FRAGMENT_SHADER, frag_src);

	GLuint program = glCreateProgram(); GLCHECK;
	GLCALL(glAttachShader(program, vs));
	GLCALL(glAttachShader(program, fs));

#if 0
	glBindAttribLocation(program, index, name); CHKGL;
#endif

	GLCALL(glLinkProgram(program));

	GLint status;
	GLCALL(glGetProgramiv(program, GL_LINK_STATUS, &status));
	if (status == GL_FALSE) {
		GLint msglen;
		GLCALL(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &msglen));
		GLchar* msg = (GLchar*) malloc(msglen + 1);
		GLCALL(glGetProgramInfoLog(program, msglen, NULL, msg));
		fprintf(stderr, "shader link error: %s\n", msg);
		abort();
	}

	// safe to detach+delete after program is built
	GLCALL(glDetachShader(program, vs));
	GLCALL(glDetachShader(program, fs));
	GLCALL(glDeleteShader(vs));
	GLCALL(glDeleteShader(fs));

	return program;
}


int main(int argc, char** argv)
{
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "SDL_Init() failed\n");
		exit(EXIT_FAILURE);
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);

	SDL_GL_SetSwapInterval(1);

	SDL_Window* window = SDL_CreateWindow("skrivemaskine", 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	assert((window != NULL) && "could not create window");

	SDL_GLContext glctx;

	glctx = SDL_GL_CreateContext(window);
	assert((glctx != NULL) && "could not create GL context");

	GLCALL(glEnable(GL_BLEND));
	GLCALL(glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO));
	//GLCALL(glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA));

	const size_t vertex_buffer_size = (1<<24);
	GLuint gl_vertex_buffer;
	GLCALL(glGenBuffers(1, &gl_vertex_buffer));
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, gl_vertex_buffer));
	GLCALL(glBufferData(GL_ARRAY_BUFFER, vertex_buffer_size, NULL, GL_STREAM_DRAW));
	struct glyph_vertex* vertex_buffer = malloc(vertex_buffer_size);
	const size_t max_vertex_count = vertex_buffer_size / sizeof(struct glyph_vertex);

	struct texture font_atlas;
	texture_load(&font_atlas, "font_atlas.png");

	GLuint glyph_prg = create_render_program(
		"#version 300 es\n"
		"precision mediump float;\n"
		"uniform mat4 u_projection;\n"
		"uniform vec4 u_color;\n"
		"in vec2 a_position;\n"
		"in vec2 a_uv;\n"
		"out vec2 v_uv;\n"
		"out vec4 v_color;\n"
		"void main()\n"
		"{\n"
		"	v_uv = a_uv;\n"
		"	v_color = u_color;\n"
		"	gl_Position = u_projection * vec4(a_position,0,1);\n"
		"}\n"
	,
		"#version 300 es\n"
		"precision mediump float;\n"
		"uniform sampler2D u_texture;\n"
		"in vec2 v_uv;\n"
		"in vec4 v_color;\n"
		"out vec4 frag_color;\n"
		"void main()\n"
		"{\n"
		"	float v = texture(u_texture, v_uv).r;\n"
		"	frag_color = v_color * v;\n"
		"}\n"
	);

	const GLint glyph_u_projection = glGetUniformLocation(glyph_prg, "u_projection"); GLCHECK;
	const GLint glyph_u_color      = glGetUniformLocation(glyph_prg, "u_color"); GLCHECK;
	const GLint glyph_u_texture    = glGetUniformLocation(glyph_prg, "u_texture"); GLCHECK;

	const GLint glyph_a_position   = glGetAttribLocation(glyph_prg,  "a_position"); GLCHECK;
	const GLint glyph_a_uv         = glGetAttribLocation(glyph_prg,  "a_uv"); GLCHECK;

	float dx=0;
	int exiting = 0;
	while (!exiting) {
		dx += 1;
		if (dx>100)dx=0;

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				exiting = 1;
			} else if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
				exiting = 1;
			} else if (event.type == SDL_EVENT_TEXT_INPUT) {
				printf("TODO handle text [%s]\n", event.text.text);
			} else if ((event.type == SDL_EVENT_KEY_DOWN) || (event.type == SDL_EVENT_KEY_UP)) {
				printf("TODO handle key %d %d\n", event.key.down, event.key.key);
			}
		}

		SDL_GL_MakeCurrent(window, glctx);
		int width=-1, height=-1;
		SDL_GetWindowSizeInPixels(window, &width, &height);
		GLCALL(glViewport(0, 0, width, height));
		GLCALL(glClearColor(.9,.7,.5,0));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT));

		int num_vertices = 0;
		struct glyph_vertex* v0 = &vertex_buffer[num_vertices++];
		struct glyph_vertex* v1 = &vertex_buffer[num_vertices++];
		struct glyph_vertex* v2 = &vertex_buffer[num_vertices++];
		uint16_t M = 65535;
		v0->x = dx+0;   v0->y = 0;   v0->u = 0;  v0->v = 0;
		v1->x = dx+500; v1->y = 0;   v1->u = M;  v1->v = 0;
		v2->x = dx+0;   v2->y = 500; v2->u = 0;  v2->v = M;

		assert(num_vertices <= max_vertex_count);

		if (num_vertices > 0) {
			GLCALL(glUseProgram(glyph_prg));
			GLCALL(glBindTexture(GL_TEXTURE_2D, font_atlas.gl_texture));
			GLCALL(glUniform1i(glyph_u_texture, 0));
			GLCALL(glUniform4f(glyph_u_color, 1, 0, 0, 1));
			#if 0
			const float left   = -width/2;
			const float right  = left + width;
			const float top    = -height/2;
			const float bottom = top + height;
			#endif
			const float left   = -width;
			const float right  = left + width*2;
			const float top    = -height;
			const float bottom = top + height*2;
			const GLfloat ortho[] = {
				2.0f/(right-left)         , 0.0f                      ,  0.0f , 0.0f,
				0.0f                      , 2.0f/(top-bottom)         ,  0.0f , 0.0f,
				0.0f                      , 0.0f                      , -1.0f , 0.0f,
				(right+left)/(left-right) , (top+bottom)/(bottom-top) ,  0.0f , 1.0f,
			};
			GLCALL(glUniformMatrix4fv(glyph_u_projection, 1, 0, ortho));
			GLCALL(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(struct glyph_vertex)*num_vertices, vertex_buffer));

			GLCALL(glEnableVertexAttribArray(glyph_a_position));
			GLCALL(glEnableVertexAttribArray(glyph_a_uv));

			GLCALL(glVertexAttribPointer(
				glyph_a_position,
				2, GL_UNSIGNED_SHORT, GL_FALSE,
				sizeof(struct glyph_vertex),
				(GLvoid*)offsetof(struct glyph_vertex, x)));

			GLCALL(glVertexAttribPointer(
				glyph_a_uv,
				2, GL_UNSIGNED_SHORT, GL_TRUE,
				sizeof(struct glyph_vertex),
				(GLvoid*)offsetof(struct glyph_vertex, u)));

			GLCALL(glDrawArrays(GL_TRIANGLES, 0, num_vertices));

			GLCALL(glDisableVertexAttribArray(glyph_a_uv));
			GLCALL(glDisableVertexAttribArray(glyph_a_position));
		}

		SDL_GL_SwapWindow(window);
	}

	return EXIT_SUCCESS;
}
