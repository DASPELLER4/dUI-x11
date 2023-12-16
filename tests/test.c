#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "../include/dui.h"

void a(){
	printf("Clicked Button\n");
}

void b(){
	printf("Clicked Label\n");
}

void c(){
	printf("Clicked Input\n");
}

int x = 0;

void d(){
	x++;
	printf("Hovered Over Label For %d Frames\n",x);
}

void e(char c){
	printf("%c\n", c);
}

uiElement_t *input;
uiElement_t *button;
screen_t *screen;

void f(){
	setElementText(getElementText(input), button, screen);
}

int main(int argc, char **argv){
	screen = createScreen(0,0,1000,1000);
	uiElement_t *label = createLabel(0,0,"Label",TEXT_M,LT_RED,DK_RED,screen->display);
	button = createButton(0,label->generic.pxheight,"Button",TEXT_M,LT_BLUE,DK_BLUE,screen->display);
	input = createInput(0,button->generic.pxheight+button->generic.y,8,TEXT_L,LT_GREEN,DK_GREEN,screen->display);
	uiElement_t *followLabel = createLabel(0,0,"cursor", TEXT_S, LT_YELLOW, DK_YELLOW, screen->display);
	addElement(label, screen);
	addElement(button, screen);
	addElement(input, screen);
	addElement(followLabel, screen);
	setOnClick(input, c);
	setOnClick(label, b);
	setOnClick(button, a);
	setOnHover(label, d);
	setOnKeyPress(input, e);
	setOnReturn(input, f);
	clock_t begin = clock();
	int frames = 0;
	while(screen->keyboard->keypressed != 'q'){
		moveElement(screen->mouse->x, screen->mouse->y, followLabel, screen);
		renderScreen(screen);
		frames++;
	}
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	double fps = ((double)frames)/time_spent;
	printf("FPS - %f\n", fps);
	closeScreen(screen);
}

