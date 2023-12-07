#ifndef SCREEN_Hi
#define SCREEN_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <X11/Xlib.h>

#include "button.h"
#include "text.h"
#include "mouse.h"
#include "input.h"
#include "keyboard.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

typedef struct{
	Display *display;
	Window window;
	Visual *visual;
	int defaultScreen;
	int bpp;
	keyboard_t *keyboard;
	mouse_t *mouse;
	text_t **text;
	button_t **buttons;
	input_t **inputs;
	int textCount;
	int buttonCount;
	int inputCount;
	int maxTextCount;
	int maxButtonCount;
	int maxInputCount;
	bool exposed;
} screen_t;

screen_t *createScreen(int x, int y, int width, int height){
	screen_t *newScreen = (screen_t*)calloc(1,sizeof(screen_t));
	newScreen->display = XOpenDisplay(NULL);
	if(newScreen->display == NULL){
		perror("Cannot open X11 display\n");
		free(newScreen);
		return NULL;
	}
	newScreen->defaultScreen = DefaultScreen(newScreen->display);
	newScreen->window = XCreateSimpleWindow(newScreen->display, RootWindow(newScreen->display, newScreen->defaultScreen), x, y, width, height, 1, BlackPixel(newScreen->display, newScreen->defaultScreen), BlackPixel(newScreen->display, newScreen->defaultScreen));
	XMapWindow(newScreen->display, newScreen->window);
	XSelectInput(newScreen->display, newScreen->window, ExposureMask | PointerMotionMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask);
	newScreen->mouse = getMouse();
	newScreen->keyboard = getKeyboard();
	newScreen->visual = DefaultVisual(newScreen->display, newScreen->defaultScreen);
	newScreen->bpp = DefaultDepth(newScreen->display, newScreen->defaultScreen)/8;
	newScreen->maxTextCount = 2;
	newScreen->text = (text_t**)calloc(newScreen->maxTextCount, sizeof(text_t*));
	newScreen->maxButtonCount = 2;
	newScreen->buttons = (button_t**)calloc(newScreen->maxButtonCount, sizeof(button_t*));
	newScreen->maxInputCount = 2;
	newScreen->inputs = (input_t**)calloc(newScreen->maxInputCount, sizeof(input_t*));
	return newScreen;
}

void closeScreen(screen_t *screen){
	for(int i = 0; i < screen->maxTextCount; i++){
		if(screen->text[i]){
			deleteTextElement(screen->text[i]);
		}
	}
	free(screen->text);
	for(int i = 0; i < screen->maxButtonCount; i++)
		if(screen->buttons[i])
			deleteButtonElement(screen->buttons[i]);
	for(int i = 0; i < screen->maxInputCount; i++){
		if(screen->inputs[i]){
			deleteInputElement(screen->inputs[i]);
		}
	}
	free(screen->inputs);
	closeMouse(screen->mouse);
	closeKeyboard(screen->keyboard);
	free(screen->buttons);
	XCloseDisplay(screen->display);
	free(screen);
	screen = NULL;
}

void addText(text_t* text, screen_t *screen){
	if(screen->textCount >= screen->maxTextCount){
		screen->maxTextCount *= 2;
		screen->text = (text_t**)realloc(screen->text, screen->maxTextCount*sizeof(text_t*));
		for(int i = screen->textCount; i < screen->maxTextCount; i++)
			screen->text[i] = NULL;
	}
	int newTextLocation = -1;
	while(screen->text[++newTextLocation]);
	screen->text[newTextLocation] = text;
	screen->textCount++;
}

void addButton(button_t* button, screen_t *screen){
	if(screen->buttonCount >= screen->maxButtonCount){
		screen->maxButtonCount *= 2;
		screen->buttons = (button_t**)realloc(screen->buttons, screen->maxButtonCount*sizeof(button_t*));
		for(int i = screen->buttonCount; i < screen->maxButtonCount; i++)
			screen->buttons[i] = NULL;
	}
	int newButtonLocation = -1;
	while(screen->buttons[++newButtonLocation]);
	screen->buttons[newButtonLocation] = button;
	screen->buttonCount++;
}

void addInput(input_t* input, screen_t *screen){
	if(screen->inputCount >= screen->maxInputCount){
		screen->maxInputCount *= 2;
		screen->inputs = (input_t**)realloc(screen->inputs, screen->maxInputCount*sizeof(input_t*));
		for(int i = screen->inputCount; i < screen->maxInputCount; i++)
			screen->inputs[i] = NULL;
	}
	int newInputLocation = -1;
	while(screen->inputs[++newInputLocation]);
	screen->inputs[newInputLocation] = input;
	screen->inputCount++;
}

void handleInput(screen_t* screen){
	XEvent newEvent;
	XWindowAttributes windowAttributes;
	XGetWindowAttributes(screen->display, screen->window, &windowAttributes);
	XCheckWindowEvent(screen->display, screen->window, ExposureMask | PointerMotionMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask, &newEvent);
	if(newEvent.type == Expose){
		screen->exposed = true;
	}
	if(screen->exposed){
		updateMouse(screen->mouse, &newEvent);
		updateKeyboard(screen->keyboard, &newEvent);
	}
	for(int i = 0; i < screen->maxButtonCount; i++){
		if(screen->buttons[i] &&
			screen->buttons[i]->x<=screen->mouse->x &&
			screen->buttons[i]->x+screen->buttons[i]->byteWidth/screen->buttons[i]->bpp>=screen->mouse->x &&
			screen->buttons[i]->y<=screen->mouse->y &&
			screen->buttons[i]->y+screen->buttons[i]->size*10>=screen->mouse->y){
			if(!screen->mouse->justClicked)
				hoverButton(screen->buttons[i]);
			else
				clickButton(screen->buttons[i]);
		} else if(screen->buttons[i] && screen->buttons[i]->currBorderColor != regularBorder)
				resetButton(screen->buttons[i]);
	}
	input_t *focused = NULL;
	for(int i = 0; i < screen->maxInputCount; i++){
		if(screen->inputs[i] &&
			screen->inputs[i]->x<=screen->mouse->x &&
			screen->inputs[i]->x+screen->inputs[i]->text->byteWidth/screen->inputs[i]->text->bpp>=screen->mouse->x &&
			screen->inputs[i]->y<=screen->mouse->y &&
			screen->inputs[i]->y+screen->inputs[i]->text->fontSize*8>=screen->mouse->y){
			if(screen->mouse->justClicked){
				screen->inputs[i]->focused = true;
				renderInput(screen->inputs[i]);
			}
		} else if(screen->mouse->justClicked && screen->inputs[i] && screen->inputs[i]->focused){
			screen->inputs[i]->focused = false;
			renderInput(screen->inputs[i]);
		}
	}
	if(screen->keyboard->keypressed)
		for(int i = 0; i < screen->maxInputCount; i++){
			if(screen->inputs[i] && screen->inputs[i]->focused){
				focused = screen->inputs[i];
				break;
			}
		}
	if(focused)
		addCharacterToInput(screen->keyboard->keypressed, focused);
}

void renderTextToScreen(text_t *text, screen_t *screen){
	if(!text->visible || !screen->exposed)
		return;
	if(text->bpp != screen->bpp){
		text->bpp = screen->bpp;
		regenerateTextBuffer(text);
	}
	bool rerender = true;
	for(int i = 0; i < text->byteWidth; i++)
		if(text->textbuffer[i]){
			rerender = false;
			break;
		}
	if(rerender)
		renderText(text);
	GC gc = XCreateGC(screen->display, screen->window, screen->defaultScreen, NULL);
	XImage *image = XCreateImage(screen->display, DefaultVisual(screen->display, screen->defaultScreen), text->bpp*8, ZPixmap, 0, text->textbuffer, text->byteWidth/text->bpp, text->fontSize*8, 8, 0);
	image->bits_per_pixel = text->bpp*8; // I wish i could justify this but i cant, XCreateImage seems to ignore the arguments its given
	image->bytes_per_line = text->byteWidth;
	XPutImage(screen->display, screen->window, gc, image, 0, 0, text->x, text->y, text->byteWidth/text->bpp, text->fontSize*8);
}

void renderButtonToScreen(button_t *button, screen_t *screen){
	if(!button->visible || !screen->exposed)
		return;
	if(button->bpp != screen->bpp){
		button->bpp = screen->bpp;
		regenerateButtonBuffer(button);
	}
	bool rerender = true;
	for(int i = 0; i < button->byteWidth; i++)
		if(button->buttonbuffer[i]){
			rerender = false;
			break;
		}
	if(rerender)
		renderButton(button);
	GC gc = XCreateGC(screen->display, screen->window, screen->defaultScreen, NULL);
	XImage *image = XCreateImage(screen->display, DefaultVisual(screen->display, screen->defaultScreen), button->bpp*8, ZPixmap, 0, button->buttonbuffer, button->byteWidth/button->bpp, button->size*10, 8, 0);
	image->bits_per_pixel = button->bpp*8; // I wish i could justify this but i cant, XCreateImage seems to ignore the arguments its given
	image->bytes_per_line = button->byteWidth;
	XPutImage(screen->display, screen->window, gc, image, 0, 0, button->x, button->y, button->byteWidth/button->bpp, button->size*10);
}

void renderInputToScreen(input_t *input, screen_t *screen){
	if(!input->visible || !screen->exposed)
		return;
	if(input->text->bpp != screen->bpp){
		input->text->bpp = screen->bpp;
		regenerateTextBuffer(input->text);
	}
	bool rerender = true;
	for(int i = 0; i < input->text->byteWidth; i++)
		if(input->text->textbuffer[i]){
			rerender = false;
			break;
		}
	if(rerender)
		renderInput(input);
	GC gc = XCreateGC(screen->display, screen->window, screen->defaultScreen, NULL);
	XImage *image = XCreateImage(screen->display, DefaultVisual(screen->display, screen->defaultScreen), input->text->bpp*8, ZPixmap, 0, input->text->textbuffer, input->text->byteWidth/input->text->bpp, input->text->fontSize*8, 8, 0);
	image->bits_per_pixel = input->text->bpp*8; // I wish i could justify this but i cant, XCreateImage seems to ignore the arguments its given
	image->bytes_per_line = input->text->byteWidth;
	XPutImage(screen->display, screen->window, gc, image, 0, 0, input->x, input->y, input->text->byteWidth/input->text->bpp, input->text->fontSize*8);
}

void renderScreen(screen_t *screen){
	handleInput(screen);
	for(int i = 0; i < screen->maxTextCount; i++){
		if(screen->text[i]){
			renderTextToScreen(screen->text[i], screen);
		}
	}
	for(int i = 0; i < screen->maxButtonCount; i++){
		if(screen->buttons[i]){
			renderButtonToScreen(screen->buttons[i], screen);
		}
	}
	for(int i = 0; i < screen->maxInputCount; i++){
		if(screen->inputs[i]){
			renderInputToScreen(screen->inputs[i], screen);
		}
	}
}

#endif
