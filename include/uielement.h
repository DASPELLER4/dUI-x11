#ifndef UIELEMENT_H
#define UIELEMENT_H

#include <stdint.h>

#include "input.h"
#include "button.h"
#include "text.h"

// buttons havent yet been done!!!

// setOnClick
// setOnReturn
// setText

typedef enum{
	InputElement,
	ButtonElement,
	LabelElement
} elementType_t;

typedef union{
	elementType_t type;
	input_t input;
	text_t text;
	button_t button;
	long pad[30]; // for creating space for type and element
} uiElement_t;

void deleteElement(uiElement_t *element){
	switch(element->type){
		case(InputElement):
			if(element->input.ximage)
				XDestroyImage(element->input.ximage);
			else
				free(element->input.text.textbuffer);
			free(element->input.text.text);
			free(element->input.text.fgColor);
			free(element->input.text.bgColor);
			free(element->input.tempText);
			free(element->input.input);
			break;
		case(ButtonElement):
			break;
		case(LabelElement):
			if(element->text.ximage)
				XDestroyImage(element->text.ximage);
			else
				free(element->text.textbuffer);
			free(element->text.text);
			free(element->text.fgColor);
			free(element->input.text.bgColor);
			break;
	}
	free(element);
	element = NULL;
}

uiElement_t *createInput(int x, int y, int width, int fontSize, uint8_t fg[3], uint8_t bg[3], int bpp){
	uiElement_t *newElement = malloc(sizeof(uiElement_t));
	newElement->type = InputElement;
	_writeInputElement(&newElement->input, x, y, width, fontSize, fg, bg, bpp);
	_renderInput(&newElement->input);
	return newElement;
}

uiElement_t *createLabel(int x, int y, char *text, int fontSize, uint8_t fg[3], uint8_t bg[3], int bpp){
	uiElement_t *newElement = malloc(sizeof(uiElement_t));
	newElement->type = LabelElement;
	_writeTextElement(&newElement->text, x, y, text, fontSize, fg, bg, bpp);
	_renderText(&newElement->text);
	return newElement;
}

uiElement_t *createButton(int x, int y, char *text){}

#endif

