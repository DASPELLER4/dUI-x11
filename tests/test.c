#include <stdio.h>
#include <stdlib.h>
#include <duix11/dui.h>

void a(){
	printf("clicked\n");
}

int main(int argc, char **argv){
	screen_t *screen = createScreen(0,0,1920,1080);
	text_t *text = createTextElement(700,100,"text",10,ORANGE,YELLOW,screen->bpp);
	button_t *button = createButtonElement(0, 0, "a",7,BLUE,GREEN,a,screen->bpp);
	input_t *input = createInputElement(90, 50, 20, 3, LT_GREEN, DK_GREEN, screen->bpp);
	addButton(button, screen);
	addInput(input, screen);
	addText(text, screen);
	printf("Bytes Per Pixel : %d\n",screen->bpp);
	while(screen->keyboard->keypressed != 'q'){
		renderScreen(screen);
	}
	closeScreen(screen);
}
