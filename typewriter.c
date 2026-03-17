#include <stdint.h>

static const int bus_size = 7;

// how long from Za=1 until we read data bus
static const int64_t STATE0_DELAY    = 5000000; // 5ms

// how long to keep ready high
static const int64_t STATE1_DURATION = 10000000; // 10ms

// pin config
// (PIN_Outputs[] are outputs from GIER to us, so they're inputs from our
// perspective. sorry for the confusion; it was carried over from the previous
// GIER board impl)

static const int PIN_Za = 23;
static const int PIN_Outputs[] = {20,16,12,7,8,25,18};

static const int PIN_Ready = 27;
static const int PIN_Lamp = 21;
static const int PIN_Inputs[] = {13,6,5,9,22,26,19};

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

// X-macro definition that links:
//  CODE: GIER character code
//  GIDX: glyph index in font atlas
//  ENUM: character flags, or CC value (that equals CODE)
//  UTF8: UTF-8 character string
//  ALT:  GIER emulator alternative UTF-8 character string
//  SCAN: SDL3 scancode for historical layout (shift/unshift info in ENUM)
#define LIST_OF_CODES \
/*CODE  GIDX  ENUM                          UTF8    ALT    SCAN                    */ \
X( 0  ,  _N , LOWER | UPPER               , " "  , NULL , SDL_SCANCODE_SPACE       ) \
X( 1  ,   0 , LOWER                       , "1"  , NULL , SDL_SCANCODE_1           ) \
X( 1  ,   1 , UPPER                       , "∨"  , "£"  , SDL_SCANCODE_1           ) \
X( 2  ,   2 , LOWER                       , "2"  , NULL , SDL_SCANCODE_2           ) \
X( 2  ,   3 , UPPER                       , "×"  , "*"  , SDL_SCANCODE_2           ) \
X( 3  ,   4 , LOWER                       , "3"  , NULL , SDL_SCANCODE_3           ) \
X( 3  ,   5 , UPPER                       , "/"  , NULL , SDL_SCANCODE_3           ) \
X( 4  ,   6 , LOWER                       , "4"  , NULL , SDL_SCANCODE_4           ) \
X( 4  ,   7 , UPPER                       , "="  , NULL , SDL_SCANCODE_4           ) \
X( 5  ,   8 , LOWER                       , "5"  , NULL , SDL_SCANCODE_5           ) \
X( 5  ,   9 , UPPER                       , ";"  , NULL , SDL_SCANCODE_5           ) \
X( 6  ,  10 , LOWER                       , "6"  , NULL , SDL_SCANCODE_6           ) \
X( 6  ,  11 , UPPER                       , "["  , NULL , SDL_SCANCODE_6           ) \
X( 7  ,  12 , LOWER                       , "7"  , NULL , SDL_SCANCODE_7           ) \
X( 7  ,  13 , UPPER                       , "]"  , NULL , SDL_SCANCODE_7           ) \
X( 8  ,  14 , LOWER                       , "8"  , NULL , SDL_SCANCODE_8           ) \
X( 8  ,  15 , UPPER                       , "("  , NULL , SDL_SCANCODE_8           ) \
X( 9  ,  16 , LOWER                       , "9"  , NULL , SDL_SCANCODE_9           ) \
X( 9  ,  17 , UPPER                       , ")"  , NULL , SDL_SCANCODE_9           ) \
X( 10 ,  _N , NOT_USED                    , NULL , NULL , _N                       ) \
X( 11 ,  _N , STOP_CODE                   , NULL , NULL , _N                       ) \
X( 12 ,  _N , NOT_USED                    , NULL , NULL , _N                       ) \
X( 13 ,  18 , LOWER                       , "å"  , NULL , SDL_SCANCODE_LEFTBRACKET ) \
X( 13 ,  19 , UPPER                       , "Å"  , NULL , SDL_SCANCODE_LEFTBRACKET ) \
X( 14 ,  20 , LOWER | STAY                , "_"  , NULL , SDL_SCANCODE_MINUS       ) \
X( 14 ,  21 , UPPER | STAY                , "|"  , NULL , SDL_SCANCODE_MINUS       ) \
X( 15 ,  _N , NOT_USED                    , NULL , NULL , _N                       ) \
X( 16 ,  22 , LOWER                       , "0"  , NULL , SDL_SCANCODE_0           ) \
X( 16 ,  23 , UPPER                       , "∧"  , "&"  , SDL_SCANCODE_0           ) \
X( 17 ,  24 , LOWER                       , "<"  , NULL , SDL_SCANCODE_EQUALS      ) \
X( 17 ,  25 , UPPER                       , ">"  , NULL , SDL_SCANCODE_EQUALS      ) \
X( 18 ,  26 , LOWER                       , "s"  , NULL , SDL_SCANCODE_S           ) \
X( 18 ,  27 , UPPER                       , "S"  , NULL , SDL_SCANCODE_S           ) \
X( 19 ,  28 , LOWER                       , "t"  , NULL , SDL_SCANCODE_T           ) \
X( 19 ,  29 , UPPER                       , "T"  , NULL , SDL_SCANCODE_T           ) \
X( 20 ,  30 , LOWER                       , "u"  , NULL , SDL_SCANCODE_U           ) \
X( 20 ,  31 , UPPER                       , "U"  , NULL , SDL_SCANCODE_U           ) \
X( 21 ,  32 , LOWER                       , "v"  , NULL , SDL_SCANCODE_V           ) \
X( 21 ,  33 , UPPER                       , "V"  , NULL , SDL_SCANCODE_V           ) \
X( 22 ,  34 , LOWER                       , "w"  , NULL , SDL_SCANCODE_W           ) \
X( 22 ,  35 , UPPER                       , "W"  , NULL , SDL_SCANCODE_W           ) \
X( 23 ,  36 , LOWER                       , "x"  , NULL , SDL_SCANCODE_X           ) \
X( 23 ,  37 , UPPER                       , "X"  , NULL , SDL_SCANCODE_X           ) \
X( 24 ,  38 , LOWER                       , "y"  , NULL , SDL_SCANCODE_Y           ) \
X( 24 ,  39 , UPPER                       , "Y"  , NULL , SDL_SCANCODE_Y           ) \
X( 25 ,  40 , LOWER                       , "z"  , NULL , SDL_SCANCODE_Z           ) \
X( 25 ,  41 , UPPER                       , "Z"  , NULL , SDL_SCANCODE_Z           ) \
X( 26 ,  _N , NOT_USED                    , NULL , NULL , _N                       ) \
X( 27 ,  42 , LOWER                       , ","  , NULL , SDL_SCANCODE_COMMA       ) \
X( 27 ,  43 , UPPER                       , "⏨"  , "'"  , SDL_SCANCODE_COMMA       ) \
X( 28 ,  _N , NOT_USED                    , NULL , NULL , _N                       ) \
X( 29 ,  _N , RED_RIBBON                  , NULL , NULL , _N                       ) \
X( 30 ,  _N , TAB                         , NULL , NULL , _N                       ) \
X( 31 ,  _N , PUNCH_OFF                   , NULL , NULL , _N                       ) \
X( 32 ,  44 , LOWER                       , "-"  , NULL , SDL_SCANCODE_SLASH       ) \
X( 32 ,  45 , UPPER                       , "+"  , NULL , SDL_SCANCODE_SLASH       ) \
X( 33 ,  46 , LOWER                       , "j"  , NULL , SDL_SCANCODE_J           ) \
X( 33 ,  47 , UPPER                       , "J"  , NULL , SDL_SCANCODE_J           ) \
X( 34 ,  48 , LOWER                       , "k"  , NULL , SDL_SCANCODE_K           ) \
X( 34 ,  49 , UPPER                       , "K"  , NULL , SDL_SCANCODE_K           ) \
X( 35 ,  50 , LOWER                       , "l"  , NULL , SDL_SCANCODE_L           ) \
X( 35 ,  51 , UPPER                       , "L"  , NULL , SDL_SCANCODE_L           ) \
X( 36 ,  52 , LOWER                       , "m"  , NULL , SDL_SCANCODE_M           ) \
X( 36 ,  53 , UPPER                       , "M"  , NULL , SDL_SCANCODE_M           ) \
X( 37 ,  54 , LOWER                       , "n"  , NULL , SDL_SCANCODE_N           ) \
X( 37 ,  55 , UPPER                       , "N"  , NULL , SDL_SCANCODE_N           ) \
X( 38 ,  56 , LOWER                       , "o"  , NULL , SDL_SCANCODE_O           ) \
X( 38 ,  57 , UPPER                       , "O"  , NULL , SDL_SCANCODE_O           ) \
X( 39 ,  58 , LOWER                       , "p"  , NULL , SDL_SCANCODE_P           ) \
X( 39 ,  59 , UPPER                       , "P"  , NULL , SDL_SCANCODE_P           ) \
X( 40 ,  60 , LOWER                       , "q"  , NULL , SDL_SCANCODE_Q           ) \
X( 40 ,  61 , UPPER                       , "Q"  , NULL , SDL_SCANCODE_Q           ) \
X( 41 ,  62 , LOWER                       , "r"  , NULL , SDL_SCANCODE_R           ) \
X( 41 ,  63 , UPPER                       , "R"  , NULL , SDL_SCANCODE_R           ) \
X( 42 ,  _N , NOT_USED                    , NULL , NULL , _N                       ) \
X( 43 ,  64 , LOWER                       , "ø"  , NULL , SDL_SCANCODE_APOSTROPHE  ) \
X( 43 ,  65 , UPPER                       , "Ø"  , NULL , SDL_SCANCODE_APOSTROPHE  ) \
X( 44 ,  _N , PUNCH_ON                    , NULL , NULL , _N                       ) \
X( 45 ,  _N , NOT_USED                    , NULL , NULL , _N                       ) \
X( 46 ,  _N , NOT_USED                    , NULL , NULL , _N                       ) \
X( 47 ,  _N , NOT_USED                    , NULL , NULL , _N                       ) \
X( 48 ,  66 , LOWER                       , "æ"  , NULL , SDL_SCANCODE_SEMICOLON   ) \
X( 48 ,  67 , UPPER                       , "Æ"  , NULL , SDL_SCANCODE_SEMICOLON   ) \
X( 49 ,  68 , LOWER                       , "a"  , NULL , SDL_SCANCODE_A           ) \
X( 49 ,  69 , UPPER                       , "A"  , NULL , SDL_SCANCODE_A           ) \
X( 50 ,  70 , LOWER                       , "b"  , NULL , SDL_SCANCODE_B           ) \
X( 50 ,  71 , UPPER                       , "B"  , NULL , SDL_SCANCODE_B           ) \
X( 51 ,  72 , LOWER                       , "c"  , NULL , SDL_SCANCODE_C           ) \
X( 51 ,  73 , UPPER                       , "C"  , NULL , SDL_SCANCODE_C           ) \
X( 52 ,  74 , LOWER                       , "d"  , NULL , SDL_SCANCODE_D           ) \
X( 52 ,  75 , UPPER                       , "D"  , NULL , SDL_SCANCODE_D           ) \
X( 53 ,  76 , LOWER                       , "e"  , NULL , SDL_SCANCODE_E           ) \
X( 53 ,  77 , UPPER                       , "E"  , NULL , SDL_SCANCODE_E           ) \
X( 54 ,  78 , LOWER                       , "f"  , NULL , SDL_SCANCODE_F           ) \
X( 54 ,  79 , UPPER                       , "F"  , NULL , SDL_SCANCODE_F           ) \
X( 55 ,  80 , LOWER                       , "g"  , NULL , SDL_SCANCODE_G           ) \
X( 55 ,  81 , UPPER                       , "G"  , NULL , SDL_SCANCODE_G           ) \
X( 56 ,  82 , LOWER                       , "h"  , NULL , SDL_SCANCODE_H           ) \
X( 56 ,  83 , UPPER                       , "H"  , NULL , SDL_SCANCODE_H           ) \
X( 57 ,  84 , LOWER                       , "i"  , NULL , SDL_SCANCODE_I           ) \
X( 57 ,  85 , UPPER                       , "I"  , NULL , SDL_SCANCODE_I           ) \
X( 58 ,  _N , SET_LOWER                   , NULL , NULL , _N                       ) \
X( 59 ,  86 , LOWER                       , "."  , NULL , SDL_SCANCODE_PERIOD      ) \
X( 59 ,  87 , UPPER                       , ":"  , NULL , SDL_SCANCODE_PERIOD      ) \
X( 60 ,  _N , SET_UPPER                   , NULL , NULL , _N                       ) \
X( 61 ,  _N , NOT_USED                    , NULL , NULL , _N                       ) \
X( 62 ,  _N , BLACK_RIBBON                , NULL , NULL , _N                       ) \
X( 63 ,  _N , TAPE_FEED                   , NULL , NULL , _N                       ) \
X( 64 ,  _N , CAR_RETURN | LOWER | UPPER  , "\n" , NULL , SDL_SCANCODE_RETURN      ) \
/*CODE  GIDX  ENUM                          UTF8    ALT    SCAN                    */

#include <assert.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
#include <stdatomic.h>
#include <ctype.h>
#include <math.h>
#include <pthread.h>

#include <GL/gl.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengles2.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define COUNT_OF(xs) (sizeof(xs)/sizeof((xs)[0]))


// GPIO support lib ripped from Raspberry Pi's Python GPIO library

#define SETUP_OK           0
#define SETUP_DEVMEM_FAIL  1
#define SETUP_MALLOC_FAIL  2
#define SETUP_MMAP_FAIL    3
#define SETUP_CPUINFO_FAIL 4
#define SETUP_NO_PERI_ADDR 5

#define INPUT  1 // is really 0 for control register!
#define OUTPUT 0 // is really 1 for control register!
#define ALT0   4

#define HIGH 1
#define LOW  0

#define PUD_OFF  0
#define PUD_DOWN 1
#define PUD_UP   2

#define BCM2708_PERI_BASE_DEFAULT   0x20000000
#define BCM2709_PERI_BASE_DEFAULT   0x3f000000
#define BCM2710_PERI_BASE_DEFAULT   0x3f000000
#define BCM2711_PERI_BASE_DEFAULT   0xfe000000
#define GPIO_BASE_OFFSET            0x200000
#define FSEL_OFFSET                 0   // 0x0000
#define SET_OFFSET                  7   // 0x001c / 4
#define CLR_OFFSET                  10  // 0x0028 / 4
#define PINLEVEL_OFFSET             13  // 0x0034 / 4
#define EVENT_DETECT_OFFSET         16  // 0x0040 / 4
#define RISING_ED_OFFSET            19  // 0x004c / 4
#define FALLING_ED_OFFSET           22  // 0x0058 / 4
#define HIGH_DETECT_OFFSET          25  // 0x0064 / 4
#define LOW_DETECT_OFFSET           28  // 0x0070 / 4
#define PULLUPDN_OFFSET             37  // 0x0094 / 4
#define PULLUPDNCLK_OFFSET          38  // 0x0098 / 4

#define PULLUPDN_OFFSET_2711_0      57
#define PULLUPDN_OFFSET_2711_1      58
#define PULLUPDN_OFFSET_2711_2      59
#define PULLUPDN_OFFSET_2711_3      60

#define PAGE_SIZE  (4*1024)
#define BLOCK_SIZE (4*1024)

static volatile uint32_t *gpio_map;

static void short_wait(void)
{
	int i;

	for (i=0; i<150; i++) {    // wait 150 cycles
		asm volatile("nop");
	}
}

static int gpio_setup(void)
{
	int mem_fd;
	uint8_t *gpio_mem;
	uint32_t peri_base = 0;
	uint32_t gpio_base;
	uint8_t ranges[12] = { 0 };
	uint8_t rev[4] = { 0 };
	uint32_t cpu = 0;
	FILE *fp;
	char buffer[1024];
	char hardware[1024];
	int found = 0;

	// try /dev/gpiomem first - this does not require root privs
	if ((mem_fd = open("/dev/gpiomem", O_RDWR|O_SYNC)) > 0)
	{
		if ((gpio_map = (uint32_t *)mmap(NULL, BLOCK_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, mem_fd, 0)) == MAP_FAILED) {
			return SETUP_MMAP_FAIL;
		} else {
			return SETUP_OK;
		}
	}

	// revert to /dev/mem method - requires root privileges

	if ((fp = fopen("/proc/device-tree/soc/ranges", "rb")) != NULL)
	{
		// get peri base from device tree
		if (fread(ranges, 1, sizeof(ranges), fp) >= 8) {
			peri_base = ranges[4] << 24 | ranges[5] << 16 | ranges[6] << 8 | ranges[7] << 0;
			if (!peri_base) {
				peri_base = ranges[8] << 24 | ranges[9] << 16 | ranges[10] << 8 | ranges[11] << 0;
			}
		}
		if ((ranges[0] != 0x7e) ||
				(ranges[1] != 0x00) ||
				(ranges[2] != 0x00) ||
				(ranges[3] != 0x00) ||
				((peri_base != BCM2708_PERI_BASE_DEFAULT) && 
				 (peri_base != BCM2709_PERI_BASE_DEFAULT) && 
				 (peri_base != BCM2711_PERI_BASE_DEFAULT))) {
			// invalid ranges file
			peri_base = 0;
		}
		fclose(fp);
	}

	// guess peri_base based on /proc/device-tree/system/linux,revision
	if (!peri_base) {
		if ((fp = fopen("/proc/device-tree/system/linux,revision", "rb")) != NULL) {
			if (fread(rev, 1, sizeof(rev), fp) == 4) {
				cpu = (rev[2] >> 4) & 0xf;
				switch (cpu) {
					case 0 : peri_base = BCM2708_PERI_BASE_DEFAULT;
							 break;
					case 1 : 
					case 2 : peri_base = BCM2709_PERI_BASE_DEFAULT;
							 break;
					case 3 : peri_base = BCM2711_PERI_BASE_DEFAULT;
							 break;
				}
			}
			fclose(fp);
		}
	}

	// guess peri_base based on /proc/cpuinfo hardware field
	if (!peri_base) {
		if ((fp = fopen("/proc/cpuinfo", "r")) == NULL)
			return SETUP_CPUINFO_FAIL;

		while(!feof(fp) && !found && fgets(buffer, sizeof(buffer), fp)) {
			sscanf(buffer, "Hardware	: %s", hardware);
			if (strcmp(hardware, "BCM2708") == 0 || strcmp(hardware, "BCM2835") == 0) {
				// pi 1 hardware
				peri_base = BCM2708_PERI_BASE_DEFAULT;
			} else if (strcmp(hardware, "BCM2709") == 0 || strcmp(hardware, "BCM2836") == 0) {
				// pi 2 hardware
				peri_base = BCM2709_PERI_BASE_DEFAULT;
			} else if (strcmp(hardware, "BCM2710") == 0 || strcmp(hardware, "BCM2837") == 0) {
				// pi 3 hardware
				peri_base = BCM2710_PERI_BASE_DEFAULT;
			} else if (strcmp(hardware, "BCM2711") == 0) {
				// pi 4 hardware
				peri_base = BCM2711_PERI_BASE_DEFAULT;
			}
		}
		fclose(fp);
	}

	if (!peri_base)
		return SETUP_NO_PERI_ADDR;

	gpio_base = peri_base + GPIO_BASE_OFFSET;

	// mmap the GPIO memory registers
	if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0)
		return SETUP_DEVMEM_FAIL;

	if ((gpio_mem = malloc(BLOCK_SIZE + (PAGE_SIZE-1))) == NULL)
		return SETUP_MALLOC_FAIL;

	#if 1
	if ((uint32_t)gpio_mem % PAGE_SIZE)
		gpio_mem += PAGE_SIZE - ((uint32_t)gpio_mem % PAGE_SIZE);
	#else
	if ((intptr_t)gpio_mem % PAGE_SIZE)
		gpio_mem += PAGE_SIZE - ((intptr_t)gpio_mem % PAGE_SIZE);
	#endif

	if ((gpio_map = (uint32_t *)mmap( (void *)gpio_mem, BLOCK_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_FIXED, mem_fd, gpio_base)) == MAP_FAILED)
		return SETUP_MMAP_FAIL;

	return SETUP_OK;
}

#if 0
static void clear_event_detect(int gpio)
{
	int offset = EVENT_DETECT_OFFSET + (gpio/32);
	int shift = (gpio%32);

	*(gpio_map+offset) |= (1 << shift);
	short_wait();
	*(gpio_map+offset) = 0;
}

static int eventdetected(int gpio)
{
	int offset, value, bit;

	offset = EVENT_DETECT_OFFSET + (gpio/32);
	bit = (1 << (gpio%32));
	value = *(gpio_map+offset) & bit;
	if (value)
		clear_event_detect(gpio);
	return value;
}

static void set_rising_event(int gpio, int enable)
{
	int offset = RISING_ED_OFFSET + (gpio/32);
	int shift = (gpio%32);

	if (enable)
		*(gpio_map+offset) |= 1 << shift;
	else
		*(gpio_map+offset) &= ~(1 << shift);
	clear_event_detect(gpio);
}

static void set_falling_event(int gpio, int enable)
{
	int offset = FALLING_ED_OFFSET + (gpio/32);
	int shift = (gpio%32);

	if (enable) {
		*(gpio_map+offset) |= (1 << shift);
		*(gpio_map+offset) = (1 << shift);
	} else {
		*(gpio_map+offset) &= ~(1 << shift);
	}
	clear_event_detect(gpio);
}

static void set_high_event(int gpio, int enable)
{
	int offset = HIGH_DETECT_OFFSET + (gpio/32);
	int shift = (gpio%32);

	if (enable)
		*(gpio_map+offset) |= (1 << shift);
	else
		*(gpio_map+offset) &= ~(1 << shift);
	clear_event_detect(gpio);
}

static void set_low_event(int gpio, int enable)
{
	int offset = LOW_DETECT_OFFSET + (gpio/32);
	int shift = (gpio%32);

	if (enable)
		*(gpio_map+offset) |= 1 << shift;
	else
		*(gpio_map+offset) &= ~(1 << shift);
	clear_event_detect(gpio);
}
#endif

static void set_pullupdn(int gpio, int pud)
{
	// Check GPIO register
	int is2711 = *(gpio_map+PULLUPDN_OFFSET_2711_3) != 0x6770696f;
	if (is2711) {
		// Pi 4 Pull-up/down method
		int pullreg = PULLUPDN_OFFSET_2711_0 + (gpio >> 4);
		int pullshift = (gpio & 0xf) << 1;
		unsigned int pullbits;
		unsigned int pull = 0;
		switch (pud) {
			case PUD_OFF:  pull = 0; break;
			case PUD_UP:   pull = 1; break;
			case PUD_DOWN: pull = 2; break;
			default:       pull = 0; // switch PUD to OFF for other values
		}
		pullbits = *(gpio_map + pullreg);
		pullbits &= ~(3 << pullshift);
		pullbits |= (pull << pullshift);
		*(gpio_map + pullreg) = pullbits;
	} else {
		// Legacy Pull-up/down method
		int clk_offset = PULLUPDNCLK_OFFSET + (gpio/32);
		int shift = (gpio%32);

		if (pud == PUD_DOWN) {
			*(gpio_map+PULLUPDN_OFFSET) = (*(gpio_map+PULLUPDN_OFFSET) & ~3) | PUD_DOWN;
		} else if (pud == PUD_UP) {
			*(gpio_map+PULLUPDN_OFFSET) = (*(gpio_map+PULLUPDN_OFFSET) & ~3) | PUD_UP;
		} else  { // pud == PUD_OFF
			*(gpio_map+PULLUPDN_OFFSET) &= ~3;
		}
		short_wait();
		*(gpio_map+clk_offset) = 1 << shift;
		short_wait();
		*(gpio_map+PULLUPDN_OFFSET) &= ~3;
		*(gpio_map+clk_offset) = 0;
	}
}

static void setup_gpio(int gpio, int direction, int pud)
{
	int offset = FSEL_OFFSET + (gpio/10);
	int shift = (gpio%10)*3;

	set_pullupdn(gpio, pud);
	if (direction == OUTPUT)
		*(gpio_map+offset) = (*(gpio_map+offset) & ~(7<<shift)) | (1<<shift);
	else  // direction == INPUT
		*(gpio_map+offset) = (*(gpio_map+offset) & ~(7<<shift));
}

#if 0
// Contribution by Eric Ptak <trouch@trouch.com>
static int gpio_function(int gpio)
{
	int offset = FSEL_OFFSET + (gpio/10);
	int shift = (gpio%10)*3;
	int value = *(gpio_map+offset);
	value >>= shift;
	value &= 7;
	return value; // 0=input, 1=output, 4=alt0
}
#endif

static void output_gpio(int gpio, int value)
{
	int offset, shift;

	if (value) // value == HIGH
		offset = SET_OFFSET + (gpio/32);
	else       // value == LOW
		offset = CLR_OFFSET + (gpio/32);

	shift = (gpio%32);

	*(gpio_map+offset) = 1 << shift;
}

static int input_gpio(int gpio)
{
	int offset, value, mask;

	offset = PINLEVEL_OFFSET + (gpio/32);
	mask = (1 << gpio%32);
	value = *(gpio_map+offset) & mask;
	return value;
}

static void gpio_cleanup(void)
{
	munmap((void *)gpio_map, BLOCK_SIZE);
}

// end of ripped GPIO lib

static int64_t time_ns(void)
{
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	return now.tv_nsec + now.tv_sec * 1000000000LL;
}

static void gier_gpio_setup(void)
{
	int e = gpio_setup();
	if (e != SETUP_OK) {
		const char* es = "???";
		if (e == SETUP_DEVMEM_FAIL  ) es = "DEVMEM_FAIL";
		if (e == SETUP_MALLOC_FAIL  ) es = "MALLOC_FAIL";
		if (e == SETUP_MMAP_FAIL    ) es = "MMAP_FAIL";
		if (e == SETUP_CPUINFO_FAIL ) es = "CPUINFO_FAIL";
		if (e == SETUP_NO_PERI_ADDR ) es = "NO_PERI_ADDR";
		fprintf(stderr, "gpio_setup() error: %s\n", es);
		exit(EXIT_FAILURE);
	}

	// setup pins
	setup_gpio(PIN_Za, INPUT, PUD_OFF);
	for (int i=0; i<bus_size; ++i) setup_gpio(PIN_Outputs[i], INPUT, PUD_UP);

	setup_gpio(PIN_Lamp, INPUT, PUD_UP);
	setup_gpio(PIN_Ready, OUTPUT, PUD_OFF);
	for (int i=0; i<bus_size; ++i) setup_gpio(PIN_Inputs[i], OUTPUT, PUD_OFF);
}

#define RINGBUF_SIZE_LOG2 (16)
#define RINGBUF_SIZE (1<<(RINGBUF_SIZE_LOG2))
#define RINGBUF_INDEX_MASK ((1<<(RINGBUF_SIZE_LOG2))-1)

struct ringbuf {
	_Atomic int read_cursor;
	_Atomic int write_cursor;
	int buffer[RINGBUF_SIZE];
};

static struct ringbuf us2gier_ringbuf, gier2us_ringbuf;

static void ringbuf_send(struct ringbuf* rb, int value)
{
	const int wc = rb->write_cursor;
	rb->buffer[wc & RINGBUF_INDEX_MASK] = value;
	atomic_store(&rb->write_cursor, (wc+1));
}

static int ringbuf_recv(struct ringbuf* rb, int* out_value)
{
	const int wc = atomic_load(&rb->write_cursor);
	const int rc = rb->read_cursor;
	if (wc <= rc) return 0;
	const int value = rb->buffer[rc & RINGBUF_INDEX_MASK];
	atomic_store(&rb->read_cursor, (rc+1));
	if (out_value) *out_value = value;
	return 1;
}

static _Atomic int gpio_thread_exiting;
static _Atomic int lamp_is_on;

static void* gier_comm_thread(void* usr)
{
	int64_t timeout0 = 0;
	int state0 = 0;

	int64_t timeout1 = 0;
	int state1 = 0;

	while (!atomic_load(&gpio_thread_exiting)) {
		const int64_t now = time_ns();

		if (state1 == 0) {
			assert(timeout1 == 0);
			const int lamp = input_gpio(PIN_Lamp);
			atomic_store(&lamp_is_on, lamp);

			if (lamp) {
				int value = -1;
				if (ringbuf_recv(&us2gier_ringbuf, &value)) {
					printf("sending value to GIER: %d\n", value);
					for (int i=0; i<bus_size; ++i) {
						output_gpio(PIN_Inputs[i], (value & (1<<i)) ? 1 : 0);
					}
					output_gpio(PIN_Ready, 1);
					timeout1 = now + STATE1_DURATION;
					state1 = 1;
				}
			}
		} else if (state1 == 1) {
			assert(timeout1 > 0);
			if (timeout1 && (now > timeout1)) {
				output_gpio(PIN_Ready, 0);
				for (int i=0; i<bus_size; ++i) output_gpio(PIN_Inputs[i], 0);
				timeout1 = 0;
				state1 = 0;
			}
		} else {
			assert(!"unexpected state1");
		}

		if (state0 == 0) {
			const int za = input_gpio(PIN_Za);
			if (za) {
				timeout0 = now + STATE0_DELAY;
				state0 = 1;
			}
		} else if ((state0 == 1) && (now > timeout0)) {
			int value = 0;
			for (int i=0; i<bus_size; ++i) {
				value |= input_gpio(PIN_Outputs[i]) ? (1<<i) : 0;
			}
			//printf("sending value\n");
			ringbuf_send(&gier2us_ringbuf, 127-value);
			state0 = 0;
			timeout0 = 0;
		}
	}

	return NULL;
}

static inline int is_valid_code(int code)
{
	return (0 <= code) && (code <= 64);
}

// maps code to..
static int code_enum[65]; // .. ENUM value (OR'd together if flags)
static int code_lower_gidx[65]; // .. GIDX value, when (ENUM & LOWER)
static int code_upper_gidx[65]; // .. GIDX value, when (ENUM & UPPER)

static int prev_code = -1;
static int prev_gidx = -1;
static void setup_next_code(int code, int gidx, int enumval, int scancode)
{
	assert(code >= prev_code);
	prev_code = code;

	assert((gidx==_N) || (gidx > prev_gidx));
	if (gidx != _N) {
		assert(gidx > prev_gidx);
		prev_gidx = gidx;
	}

	assert((0 <= code) && (code < COUNT_OF(code_enum)));
	assert((0 <= code) && (code < COUNT_OF(code_lower_gidx)));
	assert((0 <= code) && (code < COUNT_OF(code_upper_gidx)));

	if (enumval < (1<<8)) {
		assert((gidx == _N) && "control-code cannot have glyph index");
		assert(((enumval == NOT_USED) || (enumval == code)) && "enum-code mismatch");
	} else {
		assert(((code_enum[code] & 0xff) == 0) && "more than 1 CC for code?");
	}
	code_enum[code] |= enumval;
	if (enumval & LOWER) code_lower_gidx[code] = gidx;
	if (enumval & UPPER) code_upper_gidx[code] = gidx;
}

static void setup_codes(void)
{
	#define X(CODE,GIDX,ENUM,UTF8,ALT,SCAN) setup_next_code(CODE, GIDX, ENUM, SCAN);
	LIST_OF_CODES
	#undef X
	assert(prev_code==64);
	assert(prev_gidx==87);
}

struct print_glyph {
	int8_t gidx;
	int8_t ribbon;
	int16_t column;
	int16_t row;
};

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

static void printer_state_update_for_code(struct printer_state* state, int code)
{
	if (!is_valid_code(code)) return;

	int e = code_enum[code];
	switch (code) {

	case SET_LOWER:
	case SET_UPPER: {
		assert(code==e);
		state->is_upper = (code == SET_UPPER);
	}	break;

	case BLACK_RIBBON:
	case RED_RIBBON: {
		assert(code==e);
		state->ribbon = code;
	}	break;

	case CAR_RETURN:
		assert(code==(e&0x7f));
		++state->row;
		state->column = 0;
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
		break;
	}
}

#define MAX_NUM_GLYPHS (1<<20)

struct printer {
	struct printer_state state;
	int num_glyphs;
	struct print_glyph* glyphs;
	struct ringbuf* ringbuf_tee;
};

static void printer_reset(struct printer* pr)
{
	printer_state_reset(&pr->state);
	pr->num_glyphs = 0;
}

static void printer_init(struct printer* pr)
{
	memset(pr, 0, sizeof *pr);
	pr->glyphs = calloc(MAX_NUM_GLYPHS, sizeof pr->glyphs[0]);
	printer_reset(pr);
}

static void printer_begin_ringbuf_tee(struct printer* p, struct ringbuf* rb)
{
	assert((p->ringbuf_tee == NULL) && "");
	p->ringbuf_tee = rb;
}

static void printer_end_ringbuf_tee(struct printer* p)
{
	assert(p->ringbuf_tee != NULL);
	p->ringbuf_tee = NULL;
}

static void printer_push_code(struct printer* pr, int code)
{
	//printf("push code %d\n", code);
	struct printer_state* state = &pr->state;
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
			assert(pr->num_glyphs < MAX_NUM_GLYPHS);
			pr->glyphs[pr->num_glyphs++] = g;
		}
	}
	printer_state_update_for_code(state, code);
	if (pr->ringbuf_tee != NULL) ringbuf_send(pr->ringbuf_tee, code);
}

static void printer_set_upper(struct printer* pr, int is_upper)
{
	if (is_upper == pr->state.is_upper) return;
	printer_push_code(pr, is_upper ? SET_UPPER : SET_LOWER);
	pr->state.is_upper = is_upper;
}

static void printer_toggle_ribbon(struct printer* pr)
{
	pr->state.ribbon = (pr->state.ribbon == BLACK_RIBBON) ? RED_RIBBON : BLACK_RIBBON;
}

static void printer_push_utf8(struct printer* pr, const char* utf8)
{
	struct printer_state* state = &pr->state;

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
	}
	assert(p == pend);
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
			if (n < COUNT_OF(buf)) {
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

struct draw_vertex {
	float x,y;
	uint32_t color;
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

static int enable_gpio = 1;

int main(int argc, char** argv)
{
	setup_codes();

	{
		const char* e = getenv("NOGPIO");
		if ((e != NULL) && atoi(e) > 0) {
			enable_gpio = 0;
		}
	}

	pthread_t thread;
	if (enable_gpio) {
		gier_gpio_setup();
		assert(0 == pthread_create(&thread, NULL, gier_comm_thread, NULL));
	}

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

	//struct glyph_vertex* vertex_buffer = malloc(vertex_buffer_size);
	//const size_t max_vertex_count = vertex_buffer_size / sizeof(struct glyph_vertex);

	void* vertex_buffer = malloc(vertex_buffer_size);
	const size_t max_num_glyph_vertices = vertex_buffer_size / sizeof(struct glyph_vertex);
	const size_t max_num_draw_vertices  = vertex_buffer_size / sizeof(struct draw_vertex);

	struct texture font_atlas;
	texture_load(&font_atlas, "font_atlas.png");

	const GLuint glyph_prg = create_render_program(
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

	const GLuint draw_prg = create_render_program(
		"#version 300 es\n"
		"precision mediump float;\n"
		"uniform mat4 u_projection;\n"
		"in vec2 a_position;\n"
		"in vec4 a_color;\n"
		"out vec4 v_color;\n"
		"void main()\n"
		"{\n"
		"	v_color = a_color;\n"
		"	gl_Position = u_projection * vec4(a_position,0,1);\n"
		"}\n"
	,
		"#version 300 es\n"
		"precision mediump float;\n"
		"in vec4 v_color;\n"
		"out vec4 frag_color;\n"
		"void main()\n"
		"{\n"
		"	frag_color = v_color;\n"
		"}\n"
	);

	const GLint draw_u_projection = glGetUniformLocation(draw_prg, "u_projection"); GLCHECK;
	const GLint draw_a_position   = glGetAttribLocation(draw_prg,  "a_position"); GLCHECK;
	const GLint draw_a_color      = glGetAttribLocation(draw_prg,  "a_color"); GLCHECK;


	struct printer paper_printer;
	printer_init(&paper_printer);

	struct printer menu_printer;
	printer_init(&menu_printer);

	//set_keyboard_mode(TEXT_INPUT);
	set_keyboard_mode(SCANCODE);

	const int menu_cursor_period_ms = 400;
	int menu_cursor_time_ms = 0;

	int exiting = 0;
	int in_menu = 0;
	int64_t prev_time_ms = SDL_GetTicks();

	int force_lamp = !enable_gpio;

	while (!exiting) {
		const int64_t time_ms = SDL_GetTicks();
		const int delta_ms = (int)(time_ms - prev_time_ms);
		prev_time_ms = time_ms;

		menu_cursor_time_ms += delta_ms;
		while (menu_cursor_time_ms > menu_cursor_period_ms) {
			menu_cursor_time_ms -= menu_cursor_period_ms;
		}

		const int lamp = atomic_load(&lamp_is_on);

		int val;
		while (ringbuf_recv(&gier2us_ringbuf, &val)) {
			printer_push_code(&paper_printer, val);
		}

		printer_begin_ringbuf_tee(&paper_printer, &us2gier_ringbuf);

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				exiting = 1;
			} else if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
				exiting = 1;
			}

			int is_down = 0;
			int scancode = 0;

			if ((event.type == SDL_EVENT_KEY_DOWN) || (event.type == SDL_EVENT_KEY_UP)) {
				is_down = event.key.down;
				scancode = event.key.scancode;
				printer_set_upper(&paper_printer, !!(event.key.mod & SDL_KMOD_SHIFT));
			}


			if (!in_menu && (lamp || force_lamp)) {
				if (event.type == SDL_EVENT_TEXT_INPUT) {
					if (keyboard_mode == TEXT_INPUT) {
						printer_push_utf8(&paper_printer, event.text.text);
					}
				} else if ((event.type == SDL_EVENT_KEY_DOWN) || (event.type == SDL_EVENT_KEY_UP)) {
					if (keyboard_mode == SCANCODE) {
						if (is_down) {
							const int is_upper = paper_printer.state.is_upper;
							if (0) { // to allow else if expansions
							#define X(CODE,GIDX,ENUM,UTF8,ALT,SCAN) \
							} else if ((SCAN==scancode) && (((((ENUM) & LOWER) && !is_upper) || (((ENUM) & UPPER) && is_upper)))) { \
								printer_push_code(&paper_printer, CODE);
							LIST_OF_CODES
							#undef X
							}
						}
					}
				}
			}

			if (is_down && scancode == SDL_SCANCODE_ESCAPE) exiting = 1; // FIXME menu key
			if (is_down && scancode == SDL_SCANCODE_GRAVE) in_menu = !in_menu;
			if (is_down && scancode == SDL_SCANCODE_F10) force_lamp = !force_lamp;
			if (is_down && scancode == SDL_SCANCODE_F11) printer_toggle_ribbon(&paper_printer);
			if (is_down && scancode == SDL_SCANCODE_F12) toggle_keyboard_mode(); // FIXME?
		}

		printer_end_ringbuf_tee(&paper_printer);

		SDL_GL_MakeCurrent(window, glctx);
		int width=-1, height=-1;
		SDL_GetWindowSizeInPixels(window, &width, &height);
		GLCALL(glViewport(0, 0, width, height));
		if (in_menu) {
			GLCALL(glClearColor(.3,.7,.5,0));
		} else {
			GLCALL(glClearColor(.9,.85,.7,0));
		}
		GLCALL(glClear(GL_COLOR_BUFFER_BIT));

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
				const int a = (menu_cursor_period_ms / MENU_CURSOR_CHARS) / 6;
				const int ct = (menu_cursor_time_ms + menu_cursor_period_ms - i*a) % menu_cursor_period_ms;
				const int th = (menu_cursor_period_ms * 7) / 10;
				cursor[i] = ct < th ? '>' : ' ';
			}
			cursor[MENU_CURSOR_CHARS] = 0;
			printer_push_utf8(mpr, cursor);
			printer_push_utf8(mpr, "  Afslut\n");
			//printer_push_utf8(mpr, "  xyzzy\n");
			#undef MENU_CURSOR_CHARS
		}

		struct printer* pr = in_menu ? &menu_printer : &paper_printer;

		// draw text

		GLCALL(glUseProgram(glyph_prg));
		GLCALL(glBindTexture(GL_TEXTURE_2D, font_atlas.gl_texture));
		GLCALL(glUniform1i(glyph_u_texture, 0));

		for (int ribbon_pass=0; ribbon_pass<2; ++ribbon_pass) {

			int num_vertices = 0;
			struct glyph_vertex* vertices = vertex_buffer;

			const int num_glyphs = pr->num_glyphs;
			for (int i=0; i<num_glyphs; ++i) {
				struct print_glyph* g = &pr->glyphs[i];
				const int gidx = g->gidx;

				if (ribbon_pass == 0) {
					if (g->ribbon != BLACK_RIBBON) continue;
				} else if (ribbon_pass == 1) {
					if (g->ribbon != RED_RIBBON) continue;
				} else {
					assert(!"unhandled ribbon_pass");
				}

				const int gcol = gidx & 15;
				const int grow = gidx >> 4;
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
				assert(num_vertices <= (max_num_glyph_vertices-6));
				vertices[num_vertices++] = vt0;
				vertices[num_vertices++] = vt1;
				vertices[num_vertices++] = vt2;
				vertices[num_vertices++] = vt0;
				vertices[num_vertices++] = vt2;
				vertices[num_vertices++] = vt3;
			}

			if (num_vertices == 0) continue;

			if (ribbon_pass == 0) {
				GLCALL(glUniform4f(glyph_u_color, 0, 0, 0, 1));
			} else if (ribbon_pass == 1) {
				GLCALL(glUniform4f(glyph_u_color, 1, 0, 0, 1));
			} else {
				assert(!"unhandled ribbon_pass");
			}

			const float left   = 0;
			const float right  = left + width*2;
			const float top    = -height*2 + (pr->state.row+1)*64;
			const float bottom = top + height*2;

			const GLfloat ortho[] = {
				2.0f/(right-left)         , 0.0f                      ,  0.0f , 0.0f,
				0.0f                      , 2.0f/(top-bottom)         ,  0.0f , 0.0f,
				0.0f                      , 0.0f                      , -1.0f , 0.0f,
				(right+left)/(left-right) , (top+bottom)/(bottom-top) ,  0.0f , 1.0f,
			};
			GLCALL(glUniformMatrix4fv(glyph_u_projection, 1, 0, ortho));
			GLCALL(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(struct glyph_vertex)*num_vertices, vertices));

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

		// draw lamp

		GLCALL(glUseProgram(draw_prg));

		{
			int num_vertices = 0;
			struct draw_vertex* vertices = vertex_buffer;

			for (int lamp_pass=0; lamp_pass<2; ++lamp_pass) {
				uint32_t color;
				float radius;
				if (lamp_pass == 0) {
					color = 0xff000000;
					radius = 20;
				} else if (lamp_pass == 1) {
					if (!(lamp || force_lamp)) continue;
					color = force_lamp ? 0xffff80ff : 0xff00ff00;
					radius = 15;
				}

				const int num_segs = 25;
				const float margin = 30;
				const float x = width - margin;
				const float y = margin;
				for (int i=0; i<num_segs; ++i) {
					const float a1 = (float)i / (float)num_segs * M_PI * 2.0f;
					const float dx1 = radius * cosf(a1);
					const float dy1 = radius * sinf(a1);

					const float a2 = (float)(i+1) / (float)num_segs * M_PI * 2.0f;
					const float dx2 = radius * cosf(a2);
					const float dy2 = radius * sinf(a2);

					struct draw_vertex vt0 = { .x=x     , .y=y     , .color=color };
					struct draw_vertex vt1 = { .x=x+dx1 , .y=y+dy1 , .color=color };
					struct draw_vertex vt2 = { .x=x+dx2 , .y=y+dy2 , .color=color };
					assert(num_vertices <= (max_num_draw_vertices-6));
					vertices[num_vertices++] = vt0;
					vertices[num_vertices++] = vt1;
					vertices[num_vertices++] = vt2;
				}
			}

			const float left   = 0;
			const float right  = width;
			const float top    = 0;
			const float bottom = height;

			const GLfloat ortho[] = {
				2.0f/(right-left)         , 0.0f                      ,  0.0f , 0.0f,
				0.0f                      , 2.0f/(top-bottom)         ,  0.0f , 0.0f,
				0.0f                      , 0.0f                      , -1.0f , 0.0f,
				(right+left)/(left-right) , (top+bottom)/(bottom-top) ,  0.0f , 1.0f,
			};
			GLCALL(glUniformMatrix4fv(draw_u_projection, 1, 0, ortho));
			GLCALL(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(struct draw_vertex)*num_vertices, vertices));

			GLCALL(glEnableVertexAttribArray(draw_a_position));
			GLCALL(glEnableVertexAttribArray(draw_a_color));

			GLCALL(glVertexAttribPointer(
				draw_a_position,
				2, GL_FLOAT, GL_FALSE,
				sizeof(struct draw_vertex),
				(GLvoid*)offsetof(struct draw_vertex, x)));

			GLCALL(glVertexAttribPointer(
				draw_a_color,
				4, GL_UNSIGNED_BYTE, GL_TRUE,
				sizeof(struct draw_vertex),
				(GLvoid*)offsetof(struct draw_vertex, color)));

			GLCALL(glDrawArrays(GL_TRIANGLES, 0, num_vertices));

			GLCALL(glDisableVertexAttribArray(draw_a_color));
			GLCALL(glDisableVertexAttribArray(draw_a_position));
		}

		SDL_GL_SwapWindow(window);
	}


	if (enable_gpio) {
		atomic_store(&gpio_thread_exiting, 1);
		pthread_join(thread, NULL);
		gpio_cleanup();
	}

	return EXIT_SUCCESS;
}

/*
TODO
 - menu
 - play .flx
 - real font
 - sound
 - render paper (texture, holes...)
 - print head / ribbon motion blur render
*/
