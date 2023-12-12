#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

typedef struct{
	char keypressed;
} keyboard_t;

keyboard_t *getKeyboard(){
	keyboard_t *newKeyboard = calloc(1, sizeof(keyboard_t));
	return newKeyboard;
}

void closeKeyboard(keyboard_t *keyboard){
	free(keyboard);
	keyboard = NULL;
}

void updateKeyboard(keyboard_t *keyboard, XEvent *event){
	if(event && event->type == KeyPress && event->xkey.keycode < 32764){ // the less than condition is temporary to combat an error
		char keys[32];
		KeySym key;
		XLookupString(&event->xkey, keys, 8, &key, NULL);
		keyboard->keypressed = keys[0];
	} else
		keyboard->keypressed = 0;
}

#endif

