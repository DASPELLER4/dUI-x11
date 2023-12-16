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
	XImage *ximage;
	int x;
	int y;
	int pxwidth;
	int pxheight;
	bool visible;
	bool focused;
	bool display;
	void (*onClick)();
	void (*onKeyPress)(char);
	void (*onHover)();
	void (*onReturn)();
	text_t *text;
	char *input;
	char *tempText;
	int width;
	int maxInputLength;
	int cursor;
} input_t;

void _writeInputElement(input_t* inputReturn, int x, int y, int width, int fontSize, char fg[3], char bg[3], Display *display){
	inputReturn->visible = true;
	inputReturn->x = x;
	inputReturn->y = y;
	inputReturn->width = width;
	inputReturn->tempText = (char*)calloc(width+1,1);
	memset(inputReturn->tempText, 0x20, width);
	inputReturn->maxInputLength = 2;
	inputReturn->input = (char*)calloc(inputReturn->maxInputLength, 1);
	inputReturn->text = (text_t*)calloc(1,sizeof(text_t));
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
	input->text->text = (char*)malloc(strlen(input->tempText)+1);
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

void _addCharacterToInput(char c, input_t *input){
	if(c == 0x0)
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
	_renderInput(input);
}

#endif
