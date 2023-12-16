#ifndef MOUSE_H
#define MOUSE_H

#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

typedef struct{
	int x;
	int y;
	char clicked;
	char justClicked;
} mouse_t;

mouse_t *getMouse(){
	mouse_t *newMouse = calloc(1,sizeof(mouse_t));
	newMouse->x = newMouse->y = -1;
	newMouse->clicked = 0;
	newMouse->justClicked = 0;
	return newMouse;
}

void updateMouse(mouse_t *mouse, XEvent *event){
	if(event->type == MotionNotify){
		mouse->x = event->xmotion.x;
		mouse->y = event->xmotion.y;
	} else if(event->type == ButtonPress){
		mouse->x = event->xbutton.x;
		mouse->y = event->xbutton.y;
		mouse->justClicked = 1;
		mouse->clicked = 1;
	} else if(event->type == ButtonRelease){
		mouse->x = event->xbutton.x;
		mouse->y = event->xbutton.y;
		mouse->justClicked = 0;
		mouse->clicked = 0;
	} else if(mouse->clicked && mouse->justClicked)
		mouse->justClicked = 0;
}

void closeMouse(mouse_t *mouse){
	free(mouse);
	mouse = NULL;
}

#endif

