
enum {
	// misc
	_N           = -1, // nil-index

	// CCs
	SET_LOWER    = 1, // switch to lower-case
	SET_UPPER    = 2, // switch to upper-case
	BLACK_RIBBON = 3, // switch to black ribbon
	RED_RIBBON   = 4, // switch to red ribbon
	PUNCH_OFF    = 5,
	PUNCH_ON     = 6,
	TAB          = 7,
	CAR_RETURN   = 8, // CR (carriage return)
	STOP_CODE    = 9,
	TAPE_FEED    = 10,
	NOT_USED     = 0xff,

	// flags
	LOWER        = (1<<8),  // lower-case printable char
	UPPER        = (1<<9),  // upper-case printable char
	STAY         = (1<<10), // don't advance position after print

};

// CODE: GIER character code
// GIDX: glyph index in font atlas
// ENUM: flags or CC
// UTF8: UTF-8 character string
// ALT:  GIER emulator alternative UTF-8 character string
#define LIST_OF_CODES \
/*CODE  GIDX  ENUM           UTF8    ALT */ \
X( 0  ,  _N , LOWER | UPPER , " "  , NULL ) \
X( 1  ,   0 , LOWER         , "1"  , NULL ) \
X( 1  ,   1 , UPPER         , "∨"  , "£"  ) \
X( 2  ,   2 , LOWER         , "2"  , NULL ) \
X( 2  ,   3 , UPPER         , "×"  , "*"  ) \
X( 3  ,   4 , LOWER         , "3"  , NULL ) \
X( 3  ,   5 , UPPER         , "/"  , NULL ) \
X( 4  ,   6 , LOWER         , "4"  , NULL ) \
X( 4  ,   7 , UPPER         , "="  , NULL ) \
X( 5  ,   8 , LOWER         , "5"  , NULL ) \
X( 5  ,   9 , UPPER         , ";"  , NULL ) \
X( 6  ,  10 , LOWER         , "6"  , NULL ) \
X( 6  ,  11 , UPPER         , "["  , NULL ) \
X( 7  ,  12 , LOWER         , "7"  , NULL ) \
X( 7  ,  13 , UPPER         , "]"  , NULL ) \
X( 8  ,  14 , LOWER         , "8"  , NULL ) \
X( 8  ,  15 , UPPER         , "("  , NULL ) \
X( 9  ,  16 , LOWER         , "9"  , NULL ) \
X( 9  ,  17 , UPPER         , ")"  , NULL ) \
X( 10 ,  _N , NOT_USED      , NULL , NULL ) \
X( 11 ,  _N , STOP_CODE     , NULL , NULL ) \
X( 12 ,  _N , NOT_USED      , NULL , NULL ) \
X( 13 ,  18 , LOWER         , "å"  , NULL ) \
X( 13 ,  19 , UPPER         , "Å"  , NULL ) \
X( 14 ,  20 , LOWER | STAY  , "_"  , NULL ) \
X( 14 ,  21 , UPPER | STAY  , "|"  , NULL ) \
X( 15 ,  _N , NOT_USED      , NULL , NULL ) \
X( 16 ,  22 , LOWER         , "0"  , NULL ) \
X( 16 ,  23 , UPPER         , "∧"  , "&"  ) \
X( 17 ,  24 , LOWER         , "<"  , NULL ) \
X( 17 ,  25 , UPPER         , ">"  , NULL ) \
X( 18 ,  26 , LOWER         , "s"  , NULL ) \
X( 18 ,  27 , UPPER         , "S"  , NULL ) \
X( 19 ,  28 , LOWER         , "t"  , NULL ) \
X( 19 ,  29 , UPPER         , "T"  , NULL ) \
X( 20 ,  30 , LOWER         , "u"  , NULL ) \
X( 20 ,  31 , UPPER         , "U"  , NULL ) \
X( 21 ,  32 , LOWER         , "v"  , NULL ) \
X( 21 ,  33 , UPPER         , "V"  , NULL ) \
X( 22 ,  34 , LOWER         , "w"  , NULL ) \
X( 22 ,  35 , UPPER         , "W"  , NULL ) \
X( 23 ,  36 , LOWER         , "x"  , NULL ) \
X( 23 ,  37 , UPPER         , "X"  , NULL ) \
X( 24 ,  38 , LOWER         , "y"  , NULL ) \
X( 24 ,  39 , UPPER         , "Y"  , NULL ) \
X( 25 ,  40 , LOWER         , "z"  , NULL ) \
X( 25 ,  41 , UPPER         , "Z"  , NULL ) \
X( 26 ,  _N , NOT_USED      , NULL , NULL ) \
X( 27 ,  42 , LOWER         , ","  , NULL ) \
X( 27 ,  43 , UPPER         , "⏨"  , "'"  ) \
X( 28 ,  _N , NOT_USED      , NULL , NULL ) \
X( 29 ,  _N , RED_RIBBON    , NULL , NULL ) \
X( 30 ,  _N , TAB           , NULL , NULL ) \
X( 31 ,  _N , PUNCH_OFF     , NULL , NULL ) \
X( 32 ,  44 , LOWER         , "-"  , NULL ) \
X( 32 ,  45 , UPPER         , "+"  , NULL ) \
X( 33 ,  46 , LOWER         , "j"  , NULL ) \
X( 33 ,  47 , UPPER         , "J"  , NULL ) \
X( 34 ,  48 , LOWER         , "k"  , NULL ) \
X( 34 ,  49 , UPPER         , "K"  , NULL ) \
X( 35 ,  50 , LOWER         , "l"  , NULL ) \
X( 35 ,  51 , UPPER         , "L"  , NULL ) \
X( 36 ,  52 , LOWER         , "m"  , NULL ) \
X( 36 ,  53 , UPPER         , "M"  , NULL ) \
X( 37 ,  54 , LOWER         , "n"  , NULL ) \
X( 37 ,  55 , UPPER         , "N"  , NULL ) \
X( 38 ,  56 , LOWER         , "o"  , NULL ) \
X( 38 ,  57 , UPPER         , "O"  , NULL ) \
X( 39 ,  58 , LOWER         , "p"  , NULL ) \
X( 39 ,  59 , UPPER         , "P"  , NULL ) \
X( 40 ,  60 , LOWER         , "q"  , NULL ) \
X( 40 ,  61 , UPPER         , "Q"  , NULL ) \
X( 41 ,  62 , LOWER         , "r"  , NULL ) \
X( 41 ,  63 , UPPER         , "R"  , NULL ) \
X( 42 ,  _N , NOT_USED      , NULL , NULL ) \
X( 43 ,  64 , LOWER         , "ø"  , NULL ) \
X( 43 ,  65 , UPPER         , "Ø"  , NULL ) \
X( 44 ,  _N , PUNCH_ON      , NULL , NULL ) \
X( 45 ,  _N , NOT_USED      , NULL , NULL ) \
X( 46 ,  _N , NOT_USED      , NULL , NULL ) \
X( 47 ,  _N , NOT_USED      , NULL , NULL ) \
X( 48 ,  66 , LOWER         , "æ"  , NULL ) \
X( 48 ,  67 , UPPER         , "Æ"  , NULL ) \
X( 49 ,  68 , LOWER         , "a"  , NULL ) \
X( 49 ,  69 , UPPER         , "A"  , NULL ) \
X( 50 ,  70 , LOWER         , "b"  , NULL ) \
X( 50 ,  71 , UPPER         , "B"  , NULL ) \
X( 51 ,  72 , LOWER         , "c"  , NULL ) \
X( 51 ,  73 , UPPER         , "C"  , NULL ) \
X( 52 ,  74 , LOWER         , "d"  , NULL ) \
X( 52 ,  75 , UPPER         , "D"  , NULL ) \
X( 53 ,  76 , LOWER         , "e"  , NULL ) \
X( 53 ,  77 , UPPER         , "E"  , NULL ) \
X( 54 ,  78 , LOWER         , "f"  , NULL ) \
X( 54 ,  79 , UPPER         , "F"  , NULL ) \
X( 55 ,  80 , LOWER         , "g"  , NULL ) \
X( 55 ,  81 , UPPER         , "G"  , NULL ) \
X( 56 ,  82 , LOWER         , "h"  , NULL ) \
X( 56 ,  83 , UPPER         , "H"  , NULL ) \
X( 57 ,  84 , LOWER         , "i"  , NULL ) \
X( 57 ,  85 , UPPER         , "I"  , NULL ) \
X( 58 ,  _N , SET_LOWER     , NULL , NULL ) \
X( 59 ,  86 , LOWER         , "."  , NULL ) \
X( 59 ,  87 , UPPER         , ":"  , NULL ) \
X( 60 ,  _N , SET_UPPER     , NULL , NULL ) \
X( 61 ,  _N , NOT_USED      , NULL , NULL ) \
X( 62 ,  _N , BLACK_RIBBON  , NULL , NULL ) \
X( 63 ,  _N , TAPE_FEED     , NULL , NULL ) \
X( 64 ,  _N , CAR_RETURN    , NULL , NULL ) \
/*CODE  GIDX  ENUM           UTF8    ALT */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include <GL/gl.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengles2.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define ARRAY_LENGTH(xs) (sizeof(xs)/sizeof((xs)[0]))

static int code_enum[65];
static int code_lower_gidx[65];
static int code_upper_gidx[65];

static void setup_codes(void)
{
	int prev_code = -1;
	int prev_gidx = -1;
	#define X(CODE,GIDX,ENUM,UTF8,ALT) \
	{ \
		const int code = CODE; \
		assert(code >= prev_code); \
		\
		const int gidx = GIDX; \
		assert((gidx==_N) || (gidx >= prev_gidx)); \
		prev_code = code; \
		if (gidx != _N) { \
			assert(gidx > prev_gidx); \
			prev_gidx = gidx; \
		} \
		\
		const int e = ENUM; \
		if (e < (1<<8)) { \
			assert((gidx == _N) && "control-code cannot have glyph index"); \
		} else { \
			assert(((code_enum[code] & 0xff) == 0) && "more than 1 CC for code?"); \
		} \
		code_enum[code] |= e; \
		if (e & LOWER) code_lower_gidx[code] = gidx; \
		if (e & UPPER) code_upper_gidx[code] = gidx; \
		\
		const char* alt  = ALT; \
		const char* utf8 = UTF8; \
		(void)alt; \
		(void)utf8; \
	}
	LIST_OF_CODES
	#undef X
	assert(prev_code==64);
	assert(prev_gidx==87);
}

struct print_glyph {
	int column;
	int row;
	int ribbon;
	int gidx;
};

#define CODE_RINGBUF_SIZE_LOG2 (20)

struct printer {
	int column;
	int row;
	int is_upper;
	int ribbon;
	struct print_glyph* glyph_arr;
	uint8_t* ringbuf;
	int ringbuf_read_cursor, ringbuf_write_cursor;
	int busy_ms;
};

static void printer_reset(struct printer* pr)
{
	pr->column = 0;
	pr->row = 0;
	pr->is_upper = 0;
	pr->ribbon = BLACK_RIBBON;
	pr->ringbuf_read_cursor = 0;
	pr->ringbuf_write_cursor = 0;
	pr->busy_ms = 0;
	arrsetlen(pr->glyph_arr, 0);
}

static void printer_init(struct printer* pr)
{
	memset(pr, 0, sizeof *pr);
	pr->ringbuf = calloc(1<<CODE_RINGBUF_SIZE_LOG2, sizeof *pr->ringbuf);
	assert(pr->ringbuf);
	printer_reset(pr);
}

static inline int is_valid_code(int code)
{
	return (0 <= code) && (code <= 64);
}

static void printer_process_code(struct printer* pr, int code)
{
	assert((pr->busy_ms == 0) && "printer is busy");
	if (!is_valid_code(code)) return;
	const int e = code_enum[code];
	if ((e & LOWER) || (e & UPPER)) {
		const int gidx = (pr->is_upper ? code_upper_gidx : code_lower_gidx)[code];
		if (gidx != _N) {
			assert(gidx >= 0);
			struct print_glyph g = {
				.column = pr->column,
				.row = pr->row,
				.ribbon = pr->ribbon,
				.gidx = gidx,
			};
			arrput(pr->glyph_arr, g);
		}
		if (!(e & STAY)) ++pr->column;
		pr->busy_ms = 100;
	} else {
		switch (e) {

		case SET_LOWER:
		case SET_UPPER: {
			const int prev = (pr->is_upper);
			pr->is_upper = (e == SET_UPPER);
			if (pr->is_upper != prev) pr->busy_ms = 100;
		}	break;

		case BLACK_RIBBON:
		case RED_RIBBON: {
			const int prev = (pr->ribbon);
			pr->ribbon = e;
			if (pr->ribbon != prev) pr->busy_ms = 100;
		}	break;

		case CAR_RETURN:
			++pr->row;
			pr->column = 0;
			pr->busy_ms = 200;
			break;

		case TAB: // XXX?
		case PUNCH_OFF:
		case PUNCH_ON:
		case STOP_CODE:
		case TAPE_FEED:
		case NOT_USED:
			// ignore
			break;

		default:
			assert(!"unhandled CC");
		}
	}
}

static void printer_push_code(struct printer* pr, int code)
{
	if (!is_valid_code(code)) return;
	if (pr->busy_ms == 0) {
		printer_process_code(pr, code);
	} else {
		assert(pr->busy_ms > 0);
		pr->ringbuf[(pr->ringbuf_write_cursor++) & ((1<<CODE_RINGBUF_SIZE_LOG2)-1)] = code;
	}
}

static void printer_push_utf8(struct printer* pr, const char* utf8)
{
	const int restore_is_upper = pr->is_upper;
	//printer_push_code(pr, SET_LOWER); // reset case
	printer_push_code(pr, 58); // reset case
	int is_upper = 0;

	const char* p = utf8;
	const char* pend = p + strlen(p);
	while (p < pend) {
		#define X(CODE,GIDX,ENUM,UTF8,ALT) \
		{ \
			const char* s = NULL; \
			int len = -1; \
			const size_t bytes_remaining = (pend-p); \
			{ \
				const char* u = UTF8; \
				const size_t slen = u ? strlen(u) : 0; \
				if (slen && slen <= bytes_remaining && memcmp(u,p,slen)==0) { \
					s=u; len=slen; \
				} \
			} \
			{ \
				const char* a = ALT; \
				const size_t alen = a ? strlen(a) : 0; \
				if (alen && alen <= bytes_remaining && memcmp(a,p,alen)==0) { \
					s=a; len=alen; \
				} \
			} \
			if (s != NULL) { \
				assert(len > 0); \
				const int e = ENUM; \
				const int code = CODE; \
				if (s != NULL) { \
					if ((e & UPPER) && !(e & LOWER) && !is_upper) { \
						printer_push_code(pr, /*SET_UPPER*/60); \
						is_upper = 1; \
					} else if ((e & LOWER) && !(e & UPPER) && is_upper) { \
						printer_push_code(pr, /*SET_LOWER*/58); \
						is_upper = 0; \
					} \
					printer_push_code(pr, code); \
					p += len; \
					continue; \
				} \
			} \
		}
		LIST_OF_CODES
		#undef X
		int c = *(p++);
		switch (c) {
		case '\n':
			printer_push_code(pr, 64);
			break;
		case '\t':
			printer_push_code(pr, 0);
			printer_push_code(pr, 0);
			printer_push_code(pr, 0);
			break;
		default: break;
		}
	}
	assert(p == pend);
	if (is_upper != restore_is_upper) {
		printer_push_code(pr, restore_is_upper ? SET_UPPER : SET_LOWER);
	}
}

static void printer_tick(struct printer* pr, int ms)
{
	while (ms > 0) {
		if (pr->busy_ms > ms) {
			pr->busy_ms -= ms;
			assert(pr->busy_ms > 0);
			return;
		}
		ms -= pr->busy_ms;
		assert(ms >= 0);
		pr->busy_ms = 0;
		while (pr->busy_ms == 0) {
			if (pr->ringbuf_read_cursor < pr->ringbuf_write_cursor) {
				printer_process_code(pr, pr->ringbuf[(pr->ringbuf_read_cursor++) & ((1<<CODE_RINGBUF_SIZE_LOG2)-1)]);
			} else {
				assert(pr->ringbuf_read_cursor == pr->ringbuf_write_cursor);
				return;
			}
		}
	}
}

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
	setup_codes();

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

	SDL_Window* window = SDL_CreateWindow("skrivemaskine", 1920, 1080, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
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

	struct printer printer;
	printer_init(&printer);

	printer_push_utf8(&printer, "|<Hva så drengene,\nGIER i en '-er til en _b_a_j_e_r|>");

	int exiting = 0;
	int64_t prev_time_ms = SDL_GetTicks();
	while (!exiting) {
		int64_t time_ms = SDL_GetTicks();
		int64_t delta_ms = (time_ms - prev_time_ms);
		printer_tick(&printer, delta_ms);
		prev_time_ms = time_ms;

		//printf("%zd %zd\n", delta_ms, arrlen(printer.glyph_arr));

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

		const int num_glyphs = arrlen(printer.glyph_arr);
		for (int i=0; i<num_glyphs; ++i) {
			struct print_glyph* g = &printer.glyph_arr[i];
			const int gidx = g->gidx;
			const int gcol = gidx & 15; // XXX
			const int grow = gidx >> 4; // XXX
			uint16_t x0 = g->column * 32;
			uint16_t y0 = g->row * 64;
			uint16_t x1 = x0 + 64;
			uint16_t y1 = y0 + 64;
			uint16_t u0 = (65536/16) * gcol;
			uint16_t v0 = (65536/8) * grow;
			uint16_t u1 = u0 + (65536/16)-1;
			uint16_t v1 = v0 + (65536/8)-1;
			struct glyph_vertex vt0 = { .x=x0, .y=y0, .u=u0, .v=v0 };
			struct glyph_vertex vt1 = { .x=x1, .y=y0, .u=u1, .v=v0 };
			struct glyph_vertex vt2 = { .x=x1, .y=y1, .u=u1, .v=v1 };
			struct glyph_vertex vt3 = { .x=x0, .y=y1, .u=u0, .v=v1 };
			vertex_buffer[num_vertices++] = vt0;
			vertex_buffer[num_vertices++] = vt1;
			vertex_buffer[num_vertices++] = vt2;
			vertex_buffer[num_vertices++] = vt0;
			vertex_buffer[num_vertices++] = vt2;
			vertex_buffer[num_vertices++] = vt3;
			assert(num_vertices <= max_vertex_count);
		}

		if (num_vertices > 0) {
			GLCALL(glUseProgram(glyph_prg));
			GLCALL(glBindTexture(GL_TEXTURE_2D, font_atlas.gl_texture));
			GLCALL(glUniform1i(glyph_u_texture, 0));
			GLCALL(glUniform4f(glyph_u_color, 0, 0, 0, 1));
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
