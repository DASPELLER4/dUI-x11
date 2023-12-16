#ifndef SCREEN_H
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

#define moveElement(x,y,element,screen) _moveElement(x,y,element, screen->display, screen->window, screen->elements, screen->maxElementCount)
#define setElementText(text,element,screen) _setElementText(text,element, screen->display, screen->window, screen->elements, screen->maxElementCount)

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
	bool madevisible;
} screen_t;

// TODO:

screen_t *createScreen(int x, int y, int width, int height){
	screen_t *newScreen = (screen_t*)calloc(1,sizeof(screen_t));
	newScreen->display = XOpenDisplay(NULL);
	if(newScreen->display == NULL){
		perror("Cannot open X11 display\n");
		free(newScreen);
		return NULL;
	}
	newScreen->defaultScreen = DefaultScreen(newScreen->display);
	newScreen->window = XCreateSimpleWindow(newScreen->display, RootWindow(newScreen->display, newScreen->defaultScreen), x, y, width, height, 0, WhitePixel(newScreen->display, newScreen->defaultScreen), BlackPixel(newScreen->display, newScreen->defaultScreen));
	XMapWindow(newScreen->display, newScreen->window);
	XSelectInput(newScreen->display, newScreen->window, VisibilityChangeMask | ExposureMask | PointerMotionMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask);
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
	XEvent *newEvent = calloc(1,sizeof(XEvent));
	XCheckWindowEvent(screen->display, screen->window, VisibilityChangeMask | ExposureMask | PointerMotionMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask, newEvent);
	if(newEvent->type == Expose || newEvent->type == VisibilityNotify)
		screen->madevisible = true;
	else
		screen->madevisible = false;
	updateMouse(screen->mouse, newEvent);
	updateKeyboard(screen->keyboard, newEvent);
	free(newEvent);
	for(int i = 0; i < screen->maxElementCount; i++){
		if(screen->elements[i] == NULL || !screen->elements[i]->generic.visible)
			continue;
		keypressElement(screen->elements[i], screen->keyboard->keypressed);
		if(screen->mouse->x <= screen->elements[i]->generic.x + screen->elements[i]->generic.pxwidth &&
		   screen->mouse->x >= screen->elements[i]->generic.x &&
		   screen->mouse->y >= screen->elements[i]->generic.y &&
		   screen->mouse->y <= screen->elements[i]->generic.y + screen->elements[i]->generic.pxheight){
			if(screen->mouse->justClicked)
				clickElement(screen->elements[i]);
			else
				hoverElement(screen->elements[i]);
		} else if(screen->elements[i]->generic.focused && screen->mouse->justClicked)
			unfocusElement(screen->elements[i]);
		else
			nointeractElement(screen->elements[i]);
	}
}

void renderElementToScreen(uiElement_t *element, screen_t *screen){
	if(!element->generic.visible)
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
			fprintf(stderr,"Cannot render uiElement @%lx (Ui Element is of unrecognized type)\n", (long)element);
			return;
	}
	if(element->generic.ximage == NULL){
		element->generic.ximage = XCreateImage(screen->display, DefaultVisual(screen->display, screen->defaultScreen), bpp*8, ZPixmap, 0, buffer, element->generic.pxwidth, element->generic.pxheight, 8, 0);
		if(element->generic.ximage == NULL){ // welp we tried
			fprintf(stderr,"Couldn't create XImage for uiElement @%lx\n", (long)element);
			return;
		}
		element->generic.ximage->bits_per_pixel = bpp*8;
		element->generic.ximage->bytes_per_line = bpp*element->generic.pxwidth;
	}
	if(element->generic.display || screen->madevisible){
		XPutImage(screen->display, screen->window, screen->gc, element->generic.ximage, 0, 0, element->generic.x, element->generic.y, element->generic.pxwidth, element->generic.pxheight);
		element->generic.display = false;
	}
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
