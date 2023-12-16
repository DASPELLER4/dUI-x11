#ifndef UIELEMENT_H
#define UIELEMENT_H

#include <stdint.h>
#include <X11/Xlib.h>
#include <string.h>
#include <stdlib.h>

#include "input.h"
#include "button.h"
#include "text.h"

typedef enum{
	InputElement,
	ButtonElement,
	LabelElement,
	GenericElement
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
			free(element->input.text);
			free(element->input.tempText);
			free(element->input.input);
			break;
		case ButtonElement:
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
			break;
		default:
			fprintf(stderr, "Could not free uiElement @%lx\n",(long)element);
	}
	free(element);
	element = NULL;
}

// SETTERS

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
void setOnReturn(uiElement_t *element, void (*onReturn)()){ // literally only for inputs for the forseeable future
	if(!element)
		return;
	switch(element->type){
		case InputElement:
			element->input.onReturn = onReturn;
			break;
		default:
			fprintf(stderr, "Cannot set onReturn for uiElement @%lx (Ui element type doesn't support onReturn callbacks)\n",(long)element);
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

void _moveElement(int x, int y, uiElement_t *element, Display *display, Window window, uiElement_t **otherElements, int elementCount){ // not a fan of the macro expansion, probably makes errors hard to diagnose
	if(x==element->generic.x && y==element->generic.y)
		return;
	for(int i = 0; i < elementCount; i++){
		uiElement_t *otherElement = otherElements[i];
		if(otherElements[i] == NULL || otherElements[i] == element)
			continue;
		// check if elements overlap so they can be rerendered
		if((	(element->generic.y <= otherElement->generic.y+otherElement->generic.pxheight && element->generic.y >= otherElement->generic.y) ||
			((element->generic.y+element->generic.pxheight <= otherElement->generic.y+otherElement->generic.pxheight && element->generic.y+element->generic.pxheight >= otherElement->generic.y) &&
			(element->generic.x <= otherElement->generic.x+otherElement->generic.pxwidth && element->generic.x >= otherElement->generic.x)) ||
			(element->generic.x+element->generic.pxwidth <= otherElement->generic.x+otherElement->generic.pxwidth && element->generic.x+element->generic.pxwidth>= otherElement->generic.x)) ||
			(otherElement->generic.y <= element->generic.y+element->generic.pxheight && otherElement->generic.y >= element->generic.y) ||
			((otherElement->generic.y+otherElement->generic.pxheight <= element->generic.y+element->generic.pxheight && otherElement->generic.y+otherElement->generic.pxheight >= element->generic.y) &&
			(otherElement->generic.x <= element->generic.x+element->generic.pxwidth && otherElement->generic.x >= element->generic.x)) ||
			(otherElement->generic.x+otherElement->generic.pxwidth <= element->generic.x+element->generic.pxwidth && otherElement->generic.x+otherElement->generic.pxwidth >= element->generic.x)) // NIGHTMARE NIGHTMARE NIGHTMARE
			otherElements[i]->generic.display = true;
	}
	XClearArea(display, window, element->generic.x, element->generic.y, element->generic.pxwidth, element->generic.pxheight, false);
	element->generic.x = x;
	element->generic.y = y;
	element->generic.display = true;
}

void _setElementText(char *text, uiElement_t *element, Display *display, Window window, uiElement_t **otherElements, int elementCount){
	if(!element)
		return;
	XClearArea(display, window, element->generic.x, element->generic.y, element->generic.pxwidth, element->generic.pxheight, false);
	switch(element->type){ // type dependant, not very good
		case LabelElement:
			if(element->text.ximage)
				XDestroyImage(element->text.ximage);
			else
				free(element->text.textbuffer);
			element->text.ximage = NULL;
			free(element->text.text);
			_writeTextElement(&element->text, element->generic.x, element->generic.y, text, element->text.fontSize, element->text.fgColor, element->text.bgColor, display);
			_renderText(&element->text);
			break;
		case ButtonElement:
			if(element->button.ximage)
				XDestroyImage(element->button.ximage);
			else
				free(element->button.buttonbuffer);
			element->button.ximage = NULL;
			free(element->button.text->textbuffer);
			free(element->button.text->text);
			char fgColor[3], bgColor[3];
			memcpy(fgColor, element->button.text->fgColor, 3);
			memcpy(bgColor, element->button.text->bgColor, 3);
			int fontSize = element->button.text->fontSize;
			free(element->button.text);
			_writeButtonElement(&element->button, element->generic.x, element->generic.y, text, fontSize, fgColor, bgColor, display);
			_renderButton(&element->button);
			break;
		default:
			fprintf(stderr,"Cannot set text for element @%lx (Ui Element type doesn't support changing text)\n", (long)element);
			return;
	}
	element->generic.display = true;
}

// "Constructors"

uiElement_t *createInput(int x, int y, int width, int fontSize, char fg[3], char bg[3], Display *display){
	uiElement_t *newElement = calloc(1,sizeof(uiElement_t));
	_writeInputElement(&newElement->input, x, y, width, fontSize, fg, bg, display);
	_renderInput(&newElement->input);
	newElement->type = InputElement;
	newElement->input.display = true;
	return newElement;
}

uiElement_t *createLabel(int x, int y, char *text, int fontSize, char fg[3], char bg[3], Display *display){
	uiElement_t *newElement = calloc(1,sizeof(uiElement_t));
	_writeTextElement(&newElement->text, x, y, text, fontSize, fg, bg, display);
	_renderText(&newElement->text);
	newElement->type = LabelElement;
	newElement->text.display = true;
	return newElement;
}

uiElement_t *createButton(int x, int y, char *text, int size, char fg[3], char bg[3], Display *display){
	uiElement_t *newElement = calloc(1,sizeof(uiElement_t));
	_writeButtonElement(&newElement->button, x, y, text, size, fg, bg, display);
	_renderButton(&newElement->button);
	newElement->type = ButtonElement;
	newElement->button.display = true;
	return newElement;
}

// Getters

char *getElementText(uiElement_t *element){
	switch(element->type){
		case LabelElement:
			return element->text.text;
			break;
		case ButtonElement:
			return element->button.text->text;
			break;
		case InputElement:
			return element->input.input;
			break;
		default:
			fprintf(stderr,"Cannot read text for element @%lx (Ui Element type doesn't support text)\n", (long)element);
			return NULL;
	}
}

#endif

