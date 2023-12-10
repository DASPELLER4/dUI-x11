#ifndef BUTTON_H
#define BUTTON_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <X11/Xutil.h>
#include <X11/Xlib.h>

#include "text.h"

#define hoverBorder 0xA0
#define regularBorder 0x60
#define clickBorder 0xFF

typedef struct buttonStruct{
	int type;
	char *buttonbuffer;
	XImage *ximage;
	char currBorderColor;
	text_t *text;
	void (*onClick)();
	bool visible;
	int bpp;
	int x;
	int y;
	int byteWidth;
	int size;
	int pxwidth;
	int pxheight;
} button_t;

void _writeButtonElement(button_t *returnButton, int x, int y, char *text, int size, uint8_t fg[3], uint8_t bg[3], Display *display){
	returnButton->bpp = DisplayPlanes(display, DefaultScreen(display))/8;
	returnButton->x = x;
	returnButton->y = y;
	returnButton->visible = true;
	returnButton->text = calloc(1,sizeof(text_t));
	_writeTextElement(returnButton->text, 0, 0, text, size, fg, bg, display);
	returnButton->currBorderColor = regularBorder;
	returnButton->size = size;
	returnButton->byteWidth = 2*returnButton->bpp*size+returnButton->text->byteWidth;
	returnButton->buttonbuffer = calloc(returnButton->byteWidth*(size*10),1);
}

void _renderButton(button_t* button){
	bool rerender = true;
	for(int i = 0; i < button->text->byteWidth && rerender; i++)
		if(button->text->textbuffer[i]){
			rerender = false;
			break;
		}
	if(rerender)
		_renderText(button->text);
	memset(button->buttonbuffer, button->currBorderColor, button->byteWidth*(button->size*10));
	for(int i = button->size; i < button->size*9; i++){
		memcpy(button->buttonbuffer+button->size*button->bpp+i*button->byteWidth, button->text->textbuffer+(i-button->size)*button->text->byteWidth, button->text->byteWidth);
	}
}

void hoverButton(button_t* button){
	button->currBorderColor = hoverBorder;
	_renderButton(button);
}

void clickButton(button_t* button){
	button->currBorderColor = clickBorder;
	if(button->onClick)
		button->onClick();
	_renderButton(button);
}

void resetButton(button_t* button){
	button->currBorderColor = regularBorder;
	_renderButton(button);
}

#endif
