# dUI-x11

## TODO
* ~~convert RGB arrays to the default display colormap in x11 (currently it uses BGR (which is most common))~~
* ~~handleInputs for uiElements~~
* ~~allow the user to set the positions of elements, unrendering the old element and rendering the new one~~
* ~~allow the user to change the contents of elements~~
* ~~set height and widths for each element~~
* prefix each function with a D so that the user wont have collisions with the function names
* add more tests to make it easier on devs before the docs are made
* make the docs

dUI-x11 is a simple port of dUI to be able to use it in x11 amongst other windows.

dUI-x11 will be developed further from the initial dUI codebase, making dUI deprecated.

It is currently WIP, and has issues such as the lack of clearing the screen so there is a solitare style smearing effect with moving elements.

Since it is still early days and bound to many changes, documentation has not been made yet, you can however use ```tests/test.c``` for reference.

You include with ```#include <duix11/dui.h>```
and compile with ```-lX11```

## Whats currently being worked on:
* Figuring out how to improve the rendering (remove flashes caused by moving the elements)

## What I would *like* to work on:
* Element alignment, like QVBoxLayout that automatically aligns sub elements in a reasonable manner (now you can see why im working on the uiElement union)
