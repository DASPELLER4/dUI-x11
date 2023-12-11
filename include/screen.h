#ifndef SCREEN_Hi
#define SCREEN_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <X11/Xlib.h>

#include "uielement.h"
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
	GC gc;
	int defaultScreen;
	int bpp;
	keyboard_t *keyboard;
	mouse_t *mouse;
	uiElement_t **elements;
	int maxElementCount;
	bool exposed;
} screen_t;

// TODO:
// renderElementToScreen
// fix handleInput

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
	newScreen->gc = XCreateGC(newScreen->display, newScreen->window, newScreen->defaultScreen, NULL);
	newScreen->bpp = DefaultDepth(newScreen->display, newScreen->defaultScreen)/8;
	newScreen->maxElementCount = 2;
	newScreen->elements = (uiElement_t**)calloc(newScreen->maxElementCount, sizeof(uiElement_t*));
	return newScreen;
}

void closeScreen(screen_t *screen){
	for(int i = 0; i < screen->maxElementCount; i++){
		if(screen->elements[i]){
			deleteElement(screen->elements[i]);
		}
	}
	free(screen->elements);
	closeMouse(screen->mouse);
	closeKeyboard(screen->keyboard);
	XFreeGC(screen->display, screen->gc);
	XCloseDisplay(screen->display);
	free(screen);
	screen = NULL;
}

void addElement(uiElement_t *element, screen_t *screen){
	int freeSpace = -1;
	for(int i = 0; i < screen->maxElementCount; i++)
		if(screen->elements[i] == NULL){
			freeSpace = i;
			break;
		}
	if(freeSpace == -1){
		freeSpace = screen->maxElementCount;
		screen->maxElementCount *= 2;
		screen->elements = realloc(screen->elements, sizeof(uiElement_t*)*screen->maxElementCount);
		for(int i = screen->maxElementCount/2; i < screen->maxElementCount; i++)
			screen->elements[i] = NULL;
	}
	screen->elements[freeSpace] = element;
}

void handleInput(screen_t* screen){
	XEvent *newEvent = malloc(sizeof(XEvent));
	XCheckWindowEvent(screen->display, screen->window, ExposureMask | PointerMotionMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask, newEvent);
	if(newEvent->type == Expose){
		screen->exposed = true;
	}
	if(screen->exposed){
		updateMouse(screen->mouse, newEvent);
		updateKeyboard(screen->keyboard, newEvent);
	}
	free(newEvent);
	// REMEMBER TO ADD WIDTH AND HEIGHT TO ALL ELEMENTS
	// done, pxwidth and pxheight
	for(int i = 0; i < screen->maxElementCount; i++){
		if(screen->elements[i] == NULL)
			break;
		switch(screen->elements[i]->type){
			case InputElement:
				break;
			case LabelElement:
				break;
			case ButtonElement:
				break;
		}
	}
}

// commented out cause i might return

/*
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
	if(rerender){
		if(text->ximage){
			XDestroyImage(text->ximage);
			text->ximage = NULL;
		}
		renderText(text);
	}
	if(text->ximage == NULL){
		text->ximage = XCreateImage(screen->display, screen->visual, text->bpp*8, ZPixmap, 0, (char*)text->textbuffer, text->byteWidth/text->bpp, text->fontSize*8, 8, 0);
		text->ximage->bits_per_pixel = text->bpp*8; // I wish i could justify this but i cant, XCreateImage seems to ignore the arguments its given
		text->ximage->bytes_per_line = text->byteWidth;
	}
	XPutImage(screen->display, screen->window, screen->gc, text->ximage, 0, 0, text->x, text->y, text->byteWidth/text->bpp, text->fontSize*8);
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
	if(rerender){
		if(button->ximage){
			XDestroyImage(button->ximage);
			button->ximage = NULL;
		}
		renderButton(button);
	}
	if(button->ximage == NULL){
		button->ximage = XCreateImage(screen->display, screen->visual, button->bpp*8, ZPixmap, 0, button->buttonbuffer, button->byteWidth/button->bpp, button->size*10, 8, 0);
		button->ximage->bits_per_pixel = button->bpp*8; // I wish i could justify this but i cant, XCreateImage seems to ignore the arguments its given
		button->ximage->bytes_per_line = button->byteWidth;
	}
	XPutImage(screen->display, screen->window, screen->gc, button->ximage, 0, 0, button->x, button->y, button->byteWidth/button->bpp, button->size*10);
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
	if(rerender){
		if(input->ximage){
			XDestroyImage(input->ximage);
			input->ximage = NULL;
		}
		renderInput(input);
	}
	if(input->ximage == NULL){
		input->ximage = XCreateImage(screen->display, DefaultVisual(screen->display, screen->defaultScreen), input->text->bpp*8, ZPixmap, 0, (char*)input->text->textbuffer, input->text->byteWidth/input->text->bpp, input->text->fontSize*8, 8, 0);
		input->ximage->bits_per_pixel = input->text->bpp*8; // I wish i could justify this but i cant, XCreateImage seems to ignore the arguments its given
		input->ximage->bytes_per_line = input->text->byteWidth;
	}
	XPutImage(screen->display, screen->window, screen->gc, input->ximage, 0, 0, input->x, input->y, input->text->byteWidth/input->text->bpp, input->text->fontSize*8);
}
*/

void renderElementToScreen(uiElement_t *element, screen_t *screen){
	if(!screen->exposed && element->generic.visible)
		return;
	int bpp;
	char *buffer;
	switch(element->type){ // should probably do this correctly :3
		case InputElement:
			bpp = element->input.text->bpp;
			buffer = element->input.text->textbuffer;
			break;
		case ButtonElement:
			bpp = element->button.text->bpp;
			buffer = element->button.buttonbuffer;
			break;
		case LabelElement:
			bpp = element->text.bpp;
			buffer = element->text.textbuffer;
			break;
		default:
			fprintf(stderr,"Cannot render uiElement @%ld (Ui Element is of unrecognized type)\n", (long)element);
			return;
	}
	if(element->generic.ximage == NULL){
		element->generic.ximage = XCreateImage(screen->display, DefaultVisual(screen->display, screen->defaultScreen), bpp*8, ZPixmap, 0, buffer, element->generic.pxwidth, element->generic.pxheight, 8, 0);
		printf("x - %d\ny - %d\npxwidth - %d\npxheight - %d\n*ximage - %ld\nbpp - %d\nfirst color - %x\n", element->generic.x, element->generic.y, element->generic.pxwidth, element->generic.pxheight, (long)element->generic.ximage, bpp, *buffer);
		if(element->generic.ximage == NULL){ // welp we tried
			fprintf(stderr,"Couldn't create XImage for uiElement @%ld\n", (long)element);
			return;
		}
		//element->generic.ximage->bits_per_pixel = bpp*8;
		//element->generic.ximage->bytes_per_line = bpp*element->generic.pxwidth;
	}
	XPutImage(screen->display, screen->window, screen->gc, element->generic.ximage, 0, 0, element->generic.x, element->generic.y, element->generic.pxwidth, element->generic.pxheight);
}

void renderScreen(screen_t *screen){
	handleInput(screen);
	for(int i = 0; i < screen->maxElementCount; i++){
		if(screen->elements[i] == NULL)
			continue;
		renderElementToScreen(screen->elements[i], screen);
	}
	XFlush(screen->display);
}

#endif
