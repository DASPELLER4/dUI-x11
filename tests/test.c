#include <stdio.h>
#include <stdlib.h>
#include "../include/dui.h"

void a(){
	printf("clicked\n");
}

input_t *input;

void b(){
	printf("entered %s\n", input->input);
}

int main(int argc, char **argv){
	screen_t *screen = createScreen(0,0,1920,1080);
	text_t *text = createTextElement(700,100,"text",10,ORANGE,YELLOW,screen->bpp);
	button_t *button = createButtonElement(0, 0, "a",7,BLUE,GREEN,a,screen->bpp);
	text_t *text2 = createTextElement(0,92,"a",TEXT_S,WHITE,WHITE,screen->bpp);
	input = createInputElement(90, 50, 20, 3, LT_GREEN, DK_GREEN, screen->bpp);
	input->onReturn = b;
	addButton(button, screen);
	addInput(input, screen);
	addText(text, screen);
	addText(text2, screen);
	printf("Bytes Per Pixel : %d\n",screen->bpp);
	while(screen->keyboard->keypressed != 'q'){
		printf("%ld\n", sizeof(*input));
		text2->x = screen->mouse->x;
		text2->y = screen->mouse->y;
		renderScreen(screen);
	}
	closeScreen(screen);
}
