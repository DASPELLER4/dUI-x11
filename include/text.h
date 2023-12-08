#ifndef TEXT_H
#define TEXT_H

#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <X11/Xutil.h>

#include "color_macros.h"

#define TEXT_S 1
#define TEXT_M 2
#define TEXT_L 4

typedef struct{
	char *fgColor;
	char *bgColor;
	int fontSize;
	int kerning;
	char *text;
	uint8_t *textbuffer;
	XImage *ximage;
	bool visible;
	int x;
	int y;
	int byteWidth;
	int bpp;
} text_t;

const uint8_t ascii[256][8] = {{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x66,0xff,0xff,0x66,0x66,0xff,0xff,0x66},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x18,0x38,0x70,0x60,0x60,0x70,0x38,0x18},
{0x18,0x1c,0xe,0x6,0x6,0xe,0x1c,0x18},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x0,0x0,0x0,0x0,0x18,0x18,0x8,0x10},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x0,0x0,0x0,0x0,0x0,0x0,0x18,0x18},
{0xe,0xe,0x1c,0x1c,0x38,0x38,0x70,0x70},
{0x3c,0x7e,0x66,0x6e,0x76,0x66,0x7e,0x3c},
{0x38,0x78,0x78,0x18,0x18,0x18,0x7e,0x7e},
{0x3e,0xff,0xc3,0x7,0x1e,0x78,0xff,0xff},
{0x7e,0xff,0xc3,0x3,0xe,0xc3,0xff,0x7e},
{0x1e,0x3e,0x76,0xe6,0xff,0xff,0x6,0x6},
{0xff,0x80,0xfc,0xfe,0x3,0x83,0xff,0x7e},
{0x7e,0xc1,0xc1,0xfc,0xff,0xc3,0xff,0x7e},
{0xff,0xff,0x3,0x7,0xe,0x1c,0x38,0x70},
{0x3c,0x7e,0x62,0x3c,0x62,0xc1,0xe1,0x7e},
{0x7e,0xff,0xc3,0xff,0x7f,0x3,0x3,0x3},
{0x0,0x18,0x18,0x0,0x0,0x18,0x18,0x0},
{0x18,0x18,0x0,0x0,0x18,0x18,0x8,0x10},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x0,0x7e,0x7e,0x0,0x0,0x7e,0x7e,0x0},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x7e,0x66,0xe,0x1c,0x18,0x0,0x18,0x18},
{0x3c,0x66,0xda,0xa6,0xa6,0xbd,0xc3,0x7e},
{0x3c,0x3c,0x66,0x66,0x7e,0xff,0xc3,0xc3},
{0xfc,0xc6,0xc6,0xfc,0xfe,0xc3,0xc7,0xfe},
{0x7e,0xff,0xc3,0xc0,0xc0,0xc3,0xff,0x7e},
{0xfc,0xfe,0xc7,0xc3,0xc3,0xc3,0xff,0xfe},
{0xff,0xc0,0xc0,0xff,0xc0,0xc0,0xff,0xff},
{0xff,0xff,0xc0,0xc0,0xff,0xc0,0xc0,0xc0},
{0x7e,0xc3,0xc1,0xc0,0xc7,0xc3,0xff,0x7e},
{0xc3,0xc3,0xc3,0xc3,0xff,0xff,0xc3,0xc3},
{0xff,0xff,0x18,0x18,0x18,0x18,0xff,0xff},
{0xff,0xff,0x1,0x1,0x3,0xc3,0xff,0x7c},
{0xc1,0xc6,0xd8,0xe0,0xf8,0xfe,0xc7,0xc3},
{0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xff,0xff},
{0xc3,0xe7,0xff,0xdb,0xdb,0xdb,0xc3,0xc3},
{0xe3,0xe3,0xd3,0xd3,0xcb,0xcb,0xc7,0xc7},
{0x7e,0xff,0xc3,0xc3,0xc3,0xc3,0xff,0x7e},
{0xfe,0xff,0xc3,0xc3,0xff,0xfe,0xc0,0xc0},
{0x7c,0xfe,0xc6,0xc6,0xd6,0xce,0xfe,0x7d},
{0xfc,0xc6,0xc2,0xc6,0xfc,0xf0,0xcc,0xc2},
{0x7f,0xff,0xc0,0xf0,0x7e,0x1f,0xff,0xfe},
{0xff,0xff,0x18,0x18,0x18,0x18,0x18,0x18},
{0xc3,0xc3,0xc3,0xc3,0xc3,0xc3,0xff,0x7e},
{0xc3,0xc3,0xc3,0x66,0x66,0x66,0x3c,0x18},
{0xc3,0xc3,0xdb,0xdb,0xdb,0xff,0xff,0x66},
{0xc3,0xe7,0x66,0x3c,0x18,0x7e,0xe7,0xc3},
{0xc3,0xc3,0xc3,0x66,0x66,0x18,0x18,0x18},
{0xff,0xff,0x6,0x1c,0x38,0x60,0xff,0xff},
{0x3c,0x3c,0x30,0x30,0x30,0x30,0x3c,0x3c},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x3c,0x3c,0xc,0xc,0xc,0xc,0x3c,0x3c},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x0,0x0,0x0,0x0,0x0,0x0,0xff,0xff},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x3c,0x3c,0x66,0x66,0x7e,0xff,0xc3,0xc3},
{0xfc,0xc6,0xc6,0xfc,0xfe,0xc3,0xc7,0xfe},
{0x7e,0xff,0xc3,0xc0,0xc0,0xc3,0xff,0x7e},
{0xfc,0xfe,0xc7,0xc3,0xc3,0xc3,0xff,0xfe},
{0xff,0xc0,0xc0,0xff,0xc0,0xc0,0xff,0xff},
{0xff,0xff,0xc0,0xc0,0xff,0xc0,0xc0,0xc0},
{0x7e,0xc3,0xc1,0xc0,0xc7,0xc3,0xff,0x7e},
{0xc3,0xc3,0xc3,0xc3,0xff,0xff,0xc3,0xc3},
{0xff,0xff,0x18,0x18,0x18,0x18,0xff,0xff},
{0xff,0xff,0x1,0x1,0x3,0xc3,0xff,0x7c},
{0xc1,0xc6,0xd8,0xe0,0xf8,0xfe,0xc7,0xc3},
{0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xff,0xff},
{0xc3,0xe7,0xff,0xdb,0xdb,0xdb,0xc3,0xc3},
{0xe3,0xe3,0xd3,0xd3,0xcb,0xcb,0xc7,0xc7},
{0x7e,0xff,0xc3,0xc3,0xc3,0xc3,0xff,0x7e},
{0xfe,0xff,0xc3,0xc3,0xff,0xfe,0xc0,0xc0},
{0x7c,0xfe,0xc6,0xc6,0xd6,0xce,0xfe,0x7d},
{0xfc,0xc6,0xc2,0xc6,0xfc,0xf0,0xcc,0xc2},
{0x7f,0xff,0xc0,0xf0,0x7e,0x1f,0xff,0xfe},
{0xff,0xff,0x18,0x18,0x18,0x18,0x18,0x18},
{0xc3,0xc3,0xc3,0xc3,0xc3,0xc3,0xff,0x7e},
{0xc3,0xc3,0xc3,0x66,0x66,0x66,0x3c,0x18},
{0xc3,0xc3,0xdb,0xdb,0xdb,0xff,0xff,0x66},
{0xc3,0xe7,0x66,0x3c,0x18,0x7e,0xe7,0xc3},
{0xc3,0xc3,0xc3,0x66,0x66,0x18,0x18,0x18},
{0xff,0xff,0x6,0x1c,0x38,0x60,0xff,0xff},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc},
{0x33,0x33,0xcc,0xcc,0x33,0x33,0xcc,0xcc}};

text_t *createTextElement(int x, int y, char *text, int fontSize, uint8_t fg[3], uint8_t bg[3], int bpp);
void deleteTextElement(text_t *text);
void setTextText(text_t *textElem, char *text);
void regenerateTextBuffer(text_t *text);
void renderText(text_t* text);

text_t *createTextElement(int x, int y, char *text, int fontSize, uint8_t fg[3], uint8_t bg[3], int bpp){
	text_t *newText = (text_t*)calloc(sizeof(text_t),1);
	newText->x = x;
	newText->y = y;
	newText->bpp = bpp;
	newText->visible = true;
	newText->fontSize = fontSize;
	newText->fgColor = (char*)malloc(bpp);
	newText->bgColor = (char*)malloc(bpp);
	for(int i = 0; i < bpp; i++){
		newText->fgColor[i] = fg[2-i];
		newText->bgColor[i] = bg[2-i];
	}
	newText->kerning = fontSize;
	newText->text = (char*)malloc(strlen(text)+1);
	memcpy(newText->text, text, strlen(text)+1);
	newText->byteWidth = newText->kerning*strlen(text)*bpp+strlen(text)*bpp*fontSize*8;
	newText->textbuffer = (uint8_t*)calloc(newText->byteWidth*8*fontSize,1);
	return newText;
}

void deleteTextElement(text_t *text){
	text->visible = false;
	if(text->ximage)
		XDestroyImage(text->ximage);
	else
		free(text->textbuffer);
	free(text->text);
	free(text->fgColor);
	free(text->bgColor);
	free(text);
	text = NULL;
}

void setTextText(text_t *textElem, char *text){
	free(textElem->text);
	free(textElem->textbuffer);
	textElem->text = (char*)malloc(strlen(text)+1);
	memcpy(textElem->text, text, strlen(text)+1);
	textElem->byteWidth = textElem->kerning*strlen(text)*textElem->bpp+strlen(text)*textElem->bpp*textElem->fontSize*8;
	textElem->textbuffer = (uint8_t*)calloc(textElem->byteWidth*8*textElem->fontSize,1);
}

void regenerateTextBuffer(text_t *text){
	free(text->textbuffer);
	text->byteWidth = text->kerning*strlen(text->text)*text->bpp+strlen(text->text)*text->bpp*text->fontSize*8;
	text->textbuffer = (uint8_t*)calloc(text->byteWidth*8*text->fontSize,1);
}

void renderText(text_t* text){
	for(int u = 0; u < text->byteWidth*text->fontSize*8; u+=text->bpp)
		memcpy(text->textbuffer+u, text->bgColor, text->bpp);
	for(uint8_t i = 0; text->text[i]; i++){ // each character
		for(int y = 0; y<8; y++){ // each row of the character
			for(int z = 0; z < text->fontSize; z++){ // how many times to draw that row
				int pos = i*8*text->fontSize*text->bpp + i*text->kerning*text->bpp + (y*text->fontSize+z)*text->byteWidth; // starting position for that row
				for(int j = 0; j<8; j++){ // each bit in the row
					//printf("%d", (letters[(text->text[i]&0x1F)-1][y]>>(7-j))&0x1);
					if((ascii[(unsigned char)text->text[i]][y]>>(7-j))&0x1) // if the current bit is a 1
						for(int u = 0; u < text->fontSize; u++)
							memcpy(text->textbuffer+(j*text->fontSize+u)*text->bpp+pos, text->fgColor, text->bpp);
				}
			}
		}
	}
}

#endif
