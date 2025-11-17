
enum {
	// misc
	_N           = -1, // nil-index

	// CCs
	SET_LOWER    = 58, // switch to lower-case
	SET_UPPER    = 60, // switch to upper-case
	BLACK_RIBBON = 62, // switch to black ribbon
	RED_RIBBON   = 29, // switch to red ribbon
	PUNCH_OFF    = 31,
	PUNCH_ON     = 44,
	TAB          = 30,
	CAR_RETURN   = 64, // CR (carriage return)
	STOP_CODE    = 11,
	TAPE_FEED    = 63,
	NOT_USED     = 0xff,

	// flags
	LOWER        = (1<<8),  // lower-case printable char
	UPPER        = (1<<9),  // upper-case printable char
	STAY         = (1<<10), // carriage doesn't advance
};

// CODE: GIER character code
// GIDX: glyph index in font atlas
// ENUM: flags or CC
// UTF8: UTF-8 character string
// ALT:  GIER emulator alternative UTF-8 character string
// SCAN: SDL3 scancode for historical layout (shift/unshift info in ENUM)
#define LIST_OF_CODES \
/*CODE  GIDX  ENUM           UTF8    ALT    SCAN                    */ \
X( 0  ,  _N , LOWER | UPPER , " "  , NULL , SDL_SCANCODE_SPACE       ) \
X( 1  ,   0 , LOWER         , "1"  , NULL , SDL_SCANCODE_1           ) \
X( 1  ,   1 , UPPER         , "∨"  , "£"  , SDL_SCANCODE_1           ) \
X( 2  ,   2 , LOWER         , "2"  , NULL , SDL_SCANCODE_2           ) \
X( 2  ,   3 , UPPER         , "×"  , "*"  , SDL_SCANCODE_2           ) \
X( 3  ,   4 , LOWER         , "3"  , NULL , SDL_SCANCODE_3           ) \
X( 3  ,   5 , UPPER         , "/"  , NULL , SDL_SCANCODE_3           ) \
X( 4  ,   6 , LOWER         , "4"  , NULL , SDL_SCANCODE_4           ) \
X( 4  ,   7 , UPPER         , "="  , NULL , SDL_SCANCODE_4           ) \
X( 5  ,   8 , LOWER         , "5"  , NULL , SDL_SCANCODE_5           ) \
X( 5  ,   9 , UPPER         , ";"  , NULL , SDL_SCANCODE_5           ) \
X( 6  ,  10 , LOWER         , "6"  , NULL , SDL_SCANCODE_6           ) \
X( 6  ,  11 , UPPER         , "["  , NULL , SDL_SCANCODE_6           ) \
X( 7  ,  12 , LOWER         , "7"  , NULL , SDL_SCANCODE_7           ) \
X( 7  ,  13 , UPPER         , "]"  , NULL , SDL_SCANCODE_7           ) \
X( 8  ,  14 , LOWER         , "8"  , NULL , SDL_SCANCODE_8           ) \
X( 8  ,  15 , UPPER         , "("  , NULL , SDL_SCANCODE_8           ) \
X( 9  ,  16 , LOWER         , "9"  , NULL , SDL_SCANCODE_9           ) \
X( 9  ,  17 , UPPER         , ")"  , NULL , SDL_SCANCODE_9           ) \
X( 10 ,  _N , NOT_USED      , NULL , NULL , _N                       ) \
X( 11 ,  _N , STOP_CODE     , NULL , NULL , _N                       ) \
X( 12 ,  _N , NOT_USED      , NULL , NULL , _N                       ) \
X( 13 ,  18 , LOWER         , "å"  , NULL , SDL_SCANCODE_LEFTBRACKET ) \
X( 13 ,  19 , UPPER         , "Å"  , NULL , SDL_SCANCODE_LEFTBRACKET ) \
X( 14 ,  20 , LOWER | STAY  , "_"  , NULL , SDL_SCANCODE_MINUS       ) \
X( 14 ,  21 , UPPER | STAY  , "|"  , NULL , SDL_SCANCODE_MINUS       ) \
X( 15 ,  _N , NOT_USED      , NULL , NULL , _N                       ) \
X( 16 ,  22 , LOWER         , "0"  , NULL , SDL_SCANCODE_0           ) \
X( 16 ,  23 , UPPER         , "∧"  , "&"  , SDL_SCANCODE_0           ) \
X( 17 ,  24 , LOWER         , "<"  , NULL , SDL_SCANCODE_EQUALS      ) \
X( 17 ,  25 , UPPER         , ">"  , NULL , SDL_SCANCODE_EQUALS      ) \
X( 18 ,  26 , LOWER         , "s"  , NULL , SDL_SCANCODE_S           ) \
X( 18 ,  27 , UPPER         , "S"  , NULL , SDL_SCANCODE_S           ) \
X( 19 ,  28 , LOWER         , "t"  , NULL , SDL_SCANCODE_T           ) \
X( 19 ,  29 , UPPER         , "T"  , NULL , SDL_SCANCODE_T           ) \
X( 20 ,  30 , LOWER         , "u"  , NULL , SDL_SCANCODE_U           ) \
X( 20 ,  31 , UPPER         , "U"  , NULL , SDL_SCANCODE_U           ) \
X( 21 ,  32 , LOWER         , "v"  , NULL , SDL_SCANCODE_V           ) \
X( 21 ,  33 , UPPER         , "V"  , NULL , SDL_SCANCODE_V           ) \
X( 22 ,  34 , LOWER         , "w"  , NULL , SDL_SCANCODE_W           ) \
X( 22 ,  35 , UPPER         , "W"  , NULL , SDL_SCANCODE_W           ) \
X( 23 ,  36 , LOWER         , "x"  , NULL , SDL_SCANCODE_X           ) \
X( 23 ,  37 , UPPER         , "X"  , NULL , SDL_SCANCODE_X           ) \
X( 24 ,  38 , LOWER         , "y"  , NULL , SDL_SCANCODE_Y           ) \
X( 24 ,  39 , UPPER         , "Y"  , NULL , SDL_SCANCODE_Y           ) \
X( 25 ,  40 , LOWER         , "z"  , NULL , SDL_SCANCODE_Z           ) \
X( 25 ,  41 , UPPER         , "Z"  , NULL , SDL_SCANCODE_Z           ) \
X( 26 ,  _N , NOT_USED      , NULL , NULL , _N                       ) \
X( 27 ,  42 , LOWER         , ","  , NULL , SDL_SCANCODE_COMMA       ) \
X( 27 ,  43 , UPPER         , "⏨"  , "'"  , SDL_SCANCODE_COMMA       ) \
X( 28 ,  _N , NOT_USED      , NULL , NULL , _N                       ) \
X( 29 ,  _N , RED_RIBBON    , NULL , NULL , _N                       ) \
X( 30 ,  _N , TAB           , NULL , NULL , _N                       ) \
X( 31 ,  _N , PUNCH_OFF     , NULL , NULL , _N                       ) \
X( 32 ,  44 , LOWER         , "-"  , NULL , SDL_SCANCODE_SLASH       ) \
X( 32 ,  45 , UPPER         , "+"  , NULL , SDL_SCANCODE_SLASH       ) \
X( 33 ,  46 , LOWER         , "j"  , NULL , SDL_SCANCODE_J           ) \
X( 33 ,  47 , UPPER         , "J"  , NULL , SDL_SCANCODE_J           ) \
X( 34 ,  48 , LOWER         , "k"  , NULL , SDL_SCANCODE_K           ) \
X( 34 ,  49 , UPPER         , "K"  , NULL , SDL_SCANCODE_K           ) \
X( 35 ,  50 , LOWER         , "l"  , NULL , SDL_SCANCODE_L           ) \
X( 35 ,  51 , UPPER         , "L"  , NULL , SDL_SCANCODE_L           ) \
X( 36 ,  52 , LOWER         , "m"  , NULL , SDL_SCANCODE_M           ) \
X( 36 ,  53 , UPPER         , "M"  , NULL , SDL_SCANCODE_M           ) \
X( 37 ,  54 , LOWER         , "n"  , NULL , SDL_SCANCODE_N           ) \
X( 37 ,  55 , UPPER         , "N"  , NULL , SDL_SCANCODE_N           ) \
X( 38 ,  56 , LOWER         , "o"  , NULL , SDL_SCANCODE_O           ) \
X( 38 ,  57 , UPPER         , "O"  , NULL , SDL_SCANCODE_O           ) \
X( 39 ,  58 , LOWER         , "p"  , NULL , SDL_SCANCODE_P           ) \
X( 39 ,  59 , UPPER         , "P"  , NULL , SDL_SCANCODE_P           ) \
X( 40 ,  60 , LOWER         , "q"  , NULL , SDL_SCANCODE_Q           ) \
X( 40 ,  61 , UPPER         , "Q"  , NULL , SDL_SCANCODE_Q           ) \
X( 41 ,  62 , LOWER         , "r"  , NULL , SDL_SCANCODE_R           ) \
X( 41 ,  63 , UPPER         , "R"  , NULL , SDL_SCANCODE_R           ) \
X( 42 ,  _N , NOT_USED      , NULL , NULL , _N                       ) \
X( 43 ,  64 , LOWER         , "ø"  , NULL , SDL_SCANCODE_APOSTROPHE  ) \
X( 43 ,  65 , UPPER         , "Ø"  , NULL , SDL_SCANCODE_APOSTROPHE  ) \
X( 44 ,  _N , PUNCH_ON      , NULL , NULL , _N                       ) \
X( 45 ,  _N , NOT_USED      , NULL , NULL , _N                       ) \
X( 46 ,  _N , NOT_USED      , NULL , NULL , _N                       ) \
X( 47 ,  _N , NOT_USED      , NULL , NULL , _N                       ) \
X( 48 ,  66 , LOWER         , "æ"  , NULL , SDL_SCANCODE_SEMICOLON   ) \
X( 48 ,  67 , UPPER         , "Æ"  , NULL , SDL_SCANCODE_SEMICOLON   ) \
X( 49 ,  68 , LOWER         , "a"  , NULL , SDL_SCANCODE_A           ) \
X( 49 ,  69 , UPPER         , "A"  , NULL , SDL_SCANCODE_A           ) \
X( 50 ,  70 , LOWER         , "b"  , NULL , SDL_SCANCODE_B           ) \
X( 50 ,  71 , UPPER         , "B"  , NULL , SDL_SCANCODE_B           ) \
X( 51 ,  72 , LOWER         , "c"  , NULL , SDL_SCANCODE_C           ) \
X( 51 ,  73 , UPPER         , "C"  , NULL , SDL_SCANCODE_C           ) \
X( 52 ,  74 , LOWER         , "d"  , NULL , SDL_SCANCODE_D           ) \
X( 52 ,  75 , UPPER         , "D"  , NULL , SDL_SCANCODE_D           ) \
X( 53 ,  76 , LOWER         , "e"  , NULL , SDL_SCANCODE_E           ) \
X( 53 ,  77 , UPPER         , "E"  , NULL , SDL_SCANCODE_E           ) \
X( 54 ,  78 , LOWER         , "f"  , NULL , SDL_SCANCODE_F           ) \
X( 54 ,  79 , UPPER         , "F"  , NULL , SDL_SCANCODE_F           ) \
X( 55 ,  80 , LOWER         , "g"  , NULL , SDL_SCANCODE_G           ) \
X( 55 ,  81 , UPPER         , "G"  , NULL , SDL_SCANCODE_G           ) \
X( 56 ,  82 , LOWER         , "h"  , NULL , SDL_SCANCODE_H           ) \
X( 56 ,  83 , UPPER         , "H"  , NULL , SDL_SCANCODE_H           ) \
X( 57 ,  84 , LOWER         , "i"  , NULL , SDL_SCANCODE_I           ) \
X( 57 ,  85 , UPPER         , "I"  , NULL , SDL_SCANCODE_I           ) \
X( 58 ,  _N , SET_LOWER     , NULL , NULL , _N                       ) \
X( 59 ,  86 , LOWER         , "."  , NULL , SDL_SCANCODE_PERIOD      ) \
X( 59 ,  87 , UPPER         , ":"  , NULL , SDL_SCANCODE_PERIOD      ) \
X( 60 ,  _N , SET_UPPER     , NULL , NULL , _N                       ) \
X( 61 ,  _N , NOT_USED      , NULL , NULL , _N                       ) \
X( 62 ,  _N , BLACK_RIBBON  , NULL , NULL , _N                       ) \
X( 63 ,  _N , TAPE_FEED     , NULL , NULL , _N                       ) \
X( 64 ,  _N , CAR_RETURN    , NULL , NULL , _N                       ) \
/*CODE  GIDX  ENUM           UTF8    ALT    SCAN                    */

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

static inline int is_valid_code(int code)
{
	return (0 <= code) && (code <= 64);
}

static int code_enum[65];
static int code_lower_gidx[65];
static int code_upper_gidx[65];

static void setup_codes(void)
{
	int prev_code = -1;
	int prev_gidx = -1;
	#define X(CODE,GIDX,ENUM,UTF8,ALT,SCAN) \
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
			assert(((e == NOT_USED) || (e == code)) && "enum-code mismatch"); \
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

struct printer_state {
	int column;
	int row;
	int is_upper;
	int ribbon;
};

static void printer_state_reset(struct printer_state* state)
{
	memset(state, 0, sizeof *state);
	state->ribbon = BLACK_RIBBON;
}

static int printer_state_update_for_code(struct printer_state* state, int code)
{
	if (!is_valid_code(code)) return 0;

	int e = code_enum[code];
	int busy_ms = 0;
	switch (code) {

	case SET_LOWER:
	case SET_UPPER: {
		assert(code==e);
		const int prev = (state->is_upper);
		state->is_upper = (code == SET_UPPER);
		if (state->is_upper != prev) busy_ms = 100;
	}	break;

	case BLACK_RIBBON:
	case RED_RIBBON: {
		assert(code==e);
		const int prev = (state->ribbon);
		state->ribbon = code;
		if (state->ribbon != prev) busy_ms = 100;
	}	break;

	case CAR_RETURN:
		assert(code==e);
		++state->row;
		state->column = 0;
		busy_ms = 200;
		break;

	case TAB: // XXX?
	case PUNCH_OFF: //XXX?
	case PUNCH_ON: //XXX?
	case STOP_CODE:
	case TAPE_FEED:
	case NOT_USED:
		assert(code==e);
		// ignore otherwise
		break;

	default:
		if (!(e & STAY)) ++state->column;
		busy_ms = 100;
		break;
	}

	return busy_ms;
}

struct printer {
	struct printer_state state_now, state_ahead;
	struct print_glyph* glyph_arr;
	uint8_t* ringbuf;
	int ringbuf_read_cursor, ringbuf_write_cursor;
	int busy_ms;
};

static void printer_reset(struct printer* pr)
{
	printer_state_reset(&pr->state_now);
	printer_state_reset(&pr->state_ahead);
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



static void printer_process_code(struct printer* pr, int code)
{
	struct printer_state* state = &pr->state_now;
	assert((pr->busy_ms == 0) && "printer is busy");
	if (!is_valid_code(code)) return;
	const int e = code_enum[code];
	if ((e & LOWER) || (e & UPPER)) {
		const int gidx = (state->is_upper ? code_upper_gidx : code_lower_gidx)[code];
		if (gidx != _N) {
			assert(gidx >= 0);
			struct print_glyph g = {
				.column = state->column,
				.row = state->row,
				.ribbon = state->ribbon,
				.gidx = gidx,
			};
			arrput(pr->glyph_arr, g);
		}
	}
	pr->busy_ms = printer_state_update_for_code(state, code);
}

static void printer_push_code(struct printer* pr, int code)
{
	if (!is_valid_code(code)) return;
	if (pr->busy_ms == 0) {
		printer_process_code(pr, code);
	} else {
		assert(pr->busy_ms > 0);
		pr->ringbuf[(pr->ringbuf_write_cursor++) & ((1<<CODE_RINGBUF_SIZE_LOG2)-1)] = code;
		printer_state_update_for_code(&pr->state_ahead, code);
	}
}

static void printer_push_utf8(struct printer* pr, const char* utf8)
{
	struct printer_state* state = &pr->state_ahead;

	const char* p = utf8;
	const char* pend = p + strlen(p);
	while (p < pend) {
		#define X(CODE,GIDX,ENUM,UTF8,ALT,SCAN) \
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
					if ((e & UPPER) && !(e & LOWER) && !state->is_upper) { \
						printer_push_code(pr, SET_UPPER); \
						state->is_upper = 1; \
					} else if ((e & LOWER) && !(e & UPPER) && state->is_upper) { \
						printer_push_code(pr, SET_LOWER); \
						state->is_upper = 0; \
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
			printer_push_code(pr, CAR_RETURN);
			break;
		case '\t':
			printer_push_code(pr, 0);
			printer_push_code(pr, 0);
			break;
		default: break;
		}
	}
	assert(p == pend);
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

static void printer_tick_all(struct printer* pr)
{
	while (pr->busy_ms) printer_tick(pr, 1000);
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

enum keyboard_mode {
	TEXT_INPUT = 1,
	SCANCODE,
};
static enum keyboard_mode keyboard_mode;

SDL_Window* window;

static void set_keyboard_mode(enum keyboard_mode mode)
{
	switch (mode) {
	case TEXT_INPUT:
		if (keyboard_mode != mode) {
			SDL_StartTextInput(window);
		}
		break;
	case SCANCODE:
		if (keyboard_mode != mode) {
			SDL_StopTextInput(window);
		}
		break;
	default: assert(!"unhandled keyboard mode");
	}
	keyboard_mode = mode;
}

static void toggle_keyboard_mode(void)
{
	switch (keyboard_mode) {
	case TEXT_INPUT : set_keyboard_mode(SCANCODE)   ; break;
	case SCANCODE   : set_keyboard_mode(TEXT_INPUT) ; break;
	default: assert(!"unhandled keyboard mode");
	}
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

	window = SDL_CreateWindow("skrivemaskine", 1920, 1080, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
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

	struct printer paper_printer;
	printer_init(&paper_printer);

	struct printer menu_printer;
	printer_init(&menu_printer);

	//printer_push_utf8(&paper_printer, "[======|==]\n");
	//printer_push_utf8(&paper_printer, "[------|--]\n");
	//printer_push_utf8(&paper_printer, "|< Hva så drengene,\nGIER i en '-er\ntil en _b_a_j_e_r |>");

	//set_keyboard_mode(TEXT_INPUT);
	set_keyboard_mode(SCANCODE);

	const int menu_cursor_period_ms = 600;
	int menu_cursor_time_ms = 0;

	int exiting = 0;
	int in_menu = 0;
	int64_t prev_time_ms = SDL_GetTicks();
	while (!exiting) {
		const int64_t time_ms = SDL_GetTicks();
		const int delta_ms = (int)(time_ms - prev_time_ms);
		printer_tick(&paper_printer, delta_ms);
		prev_time_ms = time_ms;

		menu_cursor_time_ms += delta_ms;
		while (menu_cursor_time_ms > menu_cursor_period_ms) {
			menu_cursor_time_ms -= menu_cursor_period_ms;
		}

		//printf("%zd %zd\n", delta_ms, arrlen(paper_printer.glyph_arr));

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				exiting = 1;
			} else if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
				exiting = 1;
			}

			const int is_down = event.key.down;
			const int scancode = event.key.scancode;
			const int is_shifted = !!(event.key.mod & SDL_KMOD_SHIFT);

			if (!in_menu) {
				if (event.type == SDL_EVENT_TEXT_INPUT) {
					if (keyboard_mode == TEXT_INPUT) {
						//printf("TODO handle text [%s]\n", event.text.text);
						printer_push_utf8(&paper_printer, event.text.text);
					}
				} else if ((event.type == SDL_EVENT_KEY_DOWN) || (event.type == SDL_EVENT_KEY_UP)) {
					if (keyboard_mode == SCANCODE) {
						printer_push_code(&paper_printer, is_shifted ? SET_UPPER : SET_LOWER);
						if (is_down) {
							if (0) { // to allow else if expansions
							#define X(CODE,GIDX,ENUM,UTF8,ALT,SCAN) \
							} else if ((SCAN==scancode) && (((ENUM & LOWER) && !is_shifted) || (ENUM & UPPER) && is_shifted)) { \
								printer_push_code(&paper_printer, CODE);
							LIST_OF_CODES
							#undef X
							}
						}
						//printf("TODO handle key %d %d\n", event.key.down, event.key.key);
						//printf("TODO handle scancode down=%d scan=%d shift=%d\n", is_down, scancode, is_shifted);
					}
					// TODO handle certain keys/scancodes regardless of keyboard
					// mode, e.g. ESC for menu, arrow up/down for scroll, etc

				}
			}

			if (in_menu) {
				// TODO
			}

			if (is_down && scancode == SDL_SCANCODE_ESCAPE) exiting = 1; // FIXME menu key
			if (is_down && scancode == SDL_SCANCODE_GRAVE) in_menu = !in_menu;
			if (is_down && scancode == SDL_SCANCODE_F12) toggle_keyboard_mode(); // FIXME?
		}

		SDL_GL_MakeCurrent(window, glctx);
		int width=-1, height=-1;
		SDL_GetWindowSizeInPixels(window, &width, &height);
		GLCALL(glViewport(0, 0, width, height));
		if (in_menu) {
			GLCALL(glClearColor(.3,.7,.5,0));
		} else {
			GLCALL(glClearColor(.9,.7,.5,0));
		}
		GLCALL(glClear(GL_COLOR_BUFFER_BIT));

		int num_vertices = 0;

		if (in_menu) {
			struct printer* mpr = &menu_printer;
			printer_reset(mpr);
			//printer_push_utf8(mpr, "|< Hva så drengene,\nGIER i en '-er\ntil en _b_a_j_e_r |>");
			// XXX hvordan indsætter jeg automatisk underscore? :) skal det
			// være en printer state eventuelt?
			//printer_push_utf8(mpr, "|< Hva så drengene,\nGIER i en '-er\ntil en _b_a_j_e_r |>");
			//printer_push_utf8(mpr, "lyd: [til] fra\n");
			//printer_push_utf8(mpr, "_l_y_d_:_ _[_t_i_l_]_ _f_r_a\n");

			#define MENU_CURSOR_CHARS (5)
			char cursor[MENU_CURSOR_CHARS + 1];
			for (int i=0; i<MENU_CURSOR_CHARS; ++i) {
				const int a = (menu_cursor_period_ms / MENU_CURSOR_CHARS) / 2;
				const int ct = (menu_cursor_time_ms + menu_cursor_period_ms - i*a) % menu_cursor_period_ms;
				const int th = (menu_cursor_period_ms * 7) / 10;
				cursor[i] = ct < th ? '>' : ' ';
			}
			cursor[MENU_CURSOR_CHARS] = 0;
			printer_push_utf8(mpr, cursor);
			printer_push_utf8(mpr, "  Afslut\n");
			#undef MENU_CURSOR_CHARS
			printer_tick_all(mpr);
		}

		struct printer* pr = in_menu ? &menu_printer : &paper_printer;

		const int num_glyphs = arrlen(pr->glyph_arr);
		for (int i=0; i<num_glyphs; ++i) {
			struct print_glyph* g = &pr->glyph_arr[i];
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
			//const float left   = -width;
			const float left   = 0;
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

/*
TODO
 - menu:
   - lyd: [til] fra
   - lyd volume: [=====|====]
   - bevægelse: [vogn] typearme
   - tastatur-layout:  [historisk] operativsystem
   - afslut...
   - afspil data...
     - *.flx?
     - rescan dir? :) jeg vil jo helst ikke have implicit I/O på en RPi :) men
       faktisk: hvorfor ikke indlæse alt ved opstart?
     - tinydir.h?
     - ligger evt. også på F2/F10 eller noget..
 - menu "visuals"?
   - _ (underscore) kan bruges til valgt menupunkt
   - [====|====] eller [----|----] til sliders
   - [selected]  notselected   til enum-options
 - socket connection to *link*.py
 - klar skrivemaskine lampe... hvordan skal den vises?
 - scroll-back på piltaster
XXX
 - hvordan fungerer "echo"? er det GIER eller skrivemaskine der echo'er?
*/
