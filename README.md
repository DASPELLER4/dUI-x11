# dUI-x11
dUI-x11 is a simple port of dUI to be able to use it in x11 amongst other windows.

It is currently WIP, and has issues such as the lack of clearing the screen so there is a solitare style smearing effect with moving elements.

Most of the regular dUI documentation applies with the difference of createScreen, which now takes the x position, y position, width and height of the window as it's arguments.

You include with ```#include <duix11/dui.h>```
and compile with ```-lX11```

## Whats currently being worked on:
* Creating a ui element union, to make the program more expandable in the case I add elements like dropdowns
* "Improving Reliability"

## What I would *like* to work on:
* Element alignment, like QVBoxLayout that automatically aligns sub elements in a reasonable manner (now you can see why im working on the uiElement union)
