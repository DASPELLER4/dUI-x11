#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "text.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

typedef struct{
	int type;
	int cursor;
	char *input;
	char *tempText;
	int maxInputLength;
	text_t *text;
	int width;
	void (*onReturn)();
	void (*onKeyPress)(char);
	void (*onClick)();
	bool focused;
	bool visible;
	XImage *ximage;
	int x;
	int y;
	int pxwidth;
	int pxheight;
} input_t;

void _writeInputElement(input_t* inputReturn, int x, int y, int width, int fontSize, uint8_t fg[3], uint8_t bg[3], Display *display){
	inputReturn->visible = true;
	inputReturn->x = x;
	inputReturn->y = y;
	inputReturn->width = width;
	inputReturn->tempText = calloc(width+1,1);
	memset(inputReturn->tempText, 0x20, width);
	inputReturn->maxInputLength = 2;
	inputReturn->input = calloc(inputReturn->maxInputLength, 1);
	inputReturn->text = calloc(1,sizeof(text_t));
	_writeTextElement(inputReturn->text, 0, 0, inputReturn->tempText, fontSize, fg, bg, display);
	inputReturn->pxwidth = inputReturn->text->pxwidth;
	inputReturn->pxheight = inputReturn->text->pxheight;
}

void _renderInput(input_t *input){
	memset(input->tempText, ' ', input->width+1);
	input->tempText[input->width] = 0;
	int l = strlen(input->input);
	strncpy(input->tempText, input->input, MIN(l, input->width));
	if(input->cursor < input->width && input->focused)
		input->tempText[input->cursor] = '|';
	free(input->text->text);
	if(input->ximage)
		XDestroyImage(input->ximage);
	else
		free(input->text->textbuffer);
	input->text->text = malloc(strlen(input->tempText)+1);
	memcpy(input->text->text, input->tempText, strlen(input->tempText)+1);
	_renderText(input->text);
}

void _backspaceInput(input_t *input){
	int a = 0;
	if(input->input[input->cursor] == 0){
		input->cursor--;
		a = 1;
	}
	input->input[input->cursor--] = 0;
	if(input->cursor < 0)
		input->cursor = a = 0;
	if(a)
		input->cursor++;
	_renderInput(input);
}

void _addCharacterToInput(unsigned char c, input_t *input){
	if(!c)
		return;
	switch(c){
		case 0x8:
			_backspaceInput(input);
			return;
			break;
		case 0xD:
			if(input->onReturn)
				input->onReturn();
			return;
			break;
	}
	if(input->cursor+2 >= input->maxInputLength){
		input->maxInputLength*=2;
		input->input = realloc(input->input, input->maxInputLength);
	}
	input->input[input->cursor++] = c;
	input->input[input->cursor] = 0;
	if(input->onKeyPress)
		input->onKeyPress((char)c);
	_renderInput(input);
}

#endif
