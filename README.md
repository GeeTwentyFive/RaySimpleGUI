Super-simple [raylib](https://github.com/raysan5/raylib) GUI library for people who don't want to mess with colors, positions, margins, styles, icons, anchors, and many other shenanigans which are unnecessary for the functionality of a GUI.

It arranges all widgets in a dynamic column with all of those aforementioned things already handled.

# Features:
- Auto-scaling based on window/screen height
- Automatically chooses between centering and scrolling based on total vertical size of all added widgets relative to vertical screen space
- One-line widget adding

# Widgets:
- Gap
- Label
- Button
- Checkbox
- InputField
- **...possibly more to come...**

# Dependencies:
[raylib](https://github.com/raysan5/raylib)

# Usage:
Between [raylib](https://github.com/raysan5/raylib)'s `BeginDrawing()` & `EndDrawing()`:
1) Assemble UI by calling related widget functions (Ex.: `RaySimpleGUI__Button("Button text", &CallbackFunction)`)
2) Call `RaySimpleGUI__Draw()`

# Example:
```c
#include <stdio.h>

#include "raylib/raylib.h"

#include "RaySimpleGUI.h"



void ExampleCallback() {
	puts("Click!");
}

int bCheckboxState = 1;
char inputFieldData[64];

int main() {

	InitWindow(1280, 720, 0);

	while (!WindowShouldClose()) {

		BeginDrawing();

			RaySimpleGUI__Label("EXAMPLE LABEL");
			RaySimpleGUI__Gap();
			RaySimpleGUI__Button("EXAMPLE BUTTON", &ExampleCallback);
			RaySimpleGUI__Checkbox(&bCheckboxState);
			RaySimpleGUI__InputField(inputFieldData, 64);

			RaySimpleGUI__Draw();

		EndDrawing();

	}

	printf("\nLast checkbox state: %d\n", bCheckboxState);
	printf("\nInput field data: %s\n", inputFieldData);

	return 0;

}
```

### ^ Example output:

![](https://github.com/GeeTwentyFive/RaySimpleGUI/blob/main/docs/Example_Output.png)
