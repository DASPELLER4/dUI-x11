#ifndef UIELEMENT_H
#define UIELEMENT_H

#include <stdint.h>
#include <X11/Xlib.h>

#include "input.h"
#include "button.h"
#include "text.h"

// setText // remember the damn ximage

typedef enum{
	InputElement,
	ButtonElement,
	LabelElement,
	GenericElement // currently, this means nothing, it just might be used later
} elementType_t;

typedef union{
	int type;
	input_t input;
	text_t text;
	button_t button;
	struct{
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
	} generic; // makes it nice :3
} uiElement_t;

void deleteElement(uiElement_t *element){
	switch(element->type){
		case InputElement:
			if(element->input.ximage)
				XDestroyImage(element->input.ximage);
			else
				free(element->input.text->textbuffer);
			free(element->input.text->text);
			free(element->input.text->fgColor);
			free(element->input.text->bgColor);
			free(element->input.text);
			free(element->input.tempText);
			free(element->input.input);
			break;
		case ButtonElement:
			free(element->button.text->bgColor);
			free(element->button.text->fgColor);
			free(element->button.text->text);
			free(element->button.text->textbuffer);
			if(element->button.ximage)
				XDestroyImage(element->button.ximage);
			else
				free(element->button.buttonbuffer);
			free(element->button.text);
			break;
		case LabelElement:
			if(element->text.ximage)
				XDestroyImage(element->text.ximage);
			else
				free(element->text.textbuffer);
			free(element->text.text);
			free(element->text.fgColor);
			free(element->text.bgColor);
			break;
		default:
			fprintf(stderr, "Could not free uiElement @%lx\n",(long)element);
	}
	free(element);
	element = NULL;
}

void setOnClick(uiElement_t *element, void (*onClick)()){
	if(!element)
		return;
	switch(element->type){
		case ButtonElement:
		case LabelElement:
		case InputElement:
			element->generic.onClick = onClick;
			break;
		default:
			fprintf(stderr, "Cannot set onClick for uiElement @%lx (Ui element type doesn't support onClick callbacks)\n",(long)element);
	}
}

void setOnKeyPress(uiElement_t *element, void (*onKeyPress)(char)){
	if(!element)
		return;
	switch(element->type){
		case InputElement:
			element->generic.onKeyPress = onKeyPress;
			break;
		default:
			fprintf(stderr, "Cannot set onKeyPress for uiElement @%lx (Ui element type doesn't support onKeyPress callbacks)\n",(long)element);
	}
}

void setOnHover(uiElement_t *element, void (*onHover)){
	if(!element)
		return;
	switch(element->type){
		case ButtonElement:
		case LabelElement:
		case InputElement:
			element->generic.onHover = onHover;
			break;
		default:
			fprintf(stderr, "Cannot set onClick for uiElement @%lx (Ui element type doesn't support onClick callbacks)\n",(long)element);
	}	
}

void clickElement(uiElement_t *element){
	if(element->generic.onClick)
		element->generic.onClick();
	switch(element->type){
		case ButtonElement:
			element->button.currBorderColor = clickBorder;
			_renderButton(&element->button);
			break;
		case InputElement:
			element->input.focused = true;
			_renderInput(&element->input);
			break;
	}
}

void hoverElement(uiElement_t *element){
	if(element->generic.onHover)
		element->generic.onHover();
	switch(element->type){
		case ButtonElement:
			element->button.currBorderColor = hoverBorder;
			_renderButton(&element->button);
			element->button.display = true;
	}
}

void nointeractElement(uiElement_t *element){
	switch(element->type){
		case ButtonElement:
			if(element->button.currBorderColor != regularBorder){
				element->button.currBorderColor = regularBorder;
				_renderButton(&element->button);
				element->button.display = true;
			}
	}
}

void keypressElement(uiElement_t *element, char c){
	if(!element->generic.focused)
		return;
	if(c && element->generic.onKeyPress)
		element->generic.onKeyPress(c);
	switch(element->type){
		case InputElement:
			_addCharacterToInput(c, &element->input);
			element->input.display = true;
	}
}

void unfocusElement(uiElement_t *element){
	element->generic.focused = false;
	switch(element->type){
		case InputElement:
			_renderInput(&element->input);
			element->input.display = true;
	}
}

uiElement_t *createInput(int x, int y, int width, int fontSize, uint8_t fg[3], uint8_t bg[3], Display *display){
	uiElement_t *newElement = calloc(1,sizeof(uiElement_t));
	_writeInputElement(&newElement->input, x, y, width, fontSize, fg, bg, display);
	_renderInput(&newElement->input);
	newElement->type = InputElement;
	newElement->input.display = true;
	return newElement;
}

uiElement_t *createLabel(int x, int y, char *text, int fontSize, uint8_t fg[3], uint8_t bg[3], Display *display){
	uiElement_t *newElement = calloc(1,sizeof(uiElement_t));
	_writeTextElement(&newElement->text, x, y, text, fontSize, fg, bg, display);
	_renderText(&newElement->text);
	newElement->type = LabelElement;
	newElement->text.display = true;
	return newElement;
}

uiElement_t *createButton(int x, int y, char *text, int size, uint8_t fg[3], uint8_t bg[3], Display *display){
	uiElement_t *newElement = calloc(1,sizeof(uiElement_t));
	_writeButtonElement(&newElement->button, x, y, text, size, fg, bg, display);
	_renderButton(&newElement->button);
	newElement->type = ButtonElement;
	newElement->button.display = true;
	return newElement;
}

#endif

