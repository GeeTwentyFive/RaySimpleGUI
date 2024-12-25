Super-simple [raylib](https://github.com/raysan5/raylib) GUI library for people who don't want to mess with colors, positions, margins, styles, icons, anchors, and many other shenanigans which are unnecessary for the functionality of a GUI.

It arranges all widgets in a dynamic column with all of those aforementioned things already handled.

# Features:
- Auto-scaling based on window/screen height
- Automatically chooses between centering and scrolling based on number of widgets
- One-line widget adding

# Widgets:
- Gap
- Label
- Button
- InputField
- **...possibly more to come...**

# Dependencies:
[raylib](https://github.com/raysan5/raylib)

# Usage:
Between [raylib](https://github.com/raysan5/raylib)'s `BeginDrawing()` & `EndDrawing()`:
1) Assemble UI by calling related widget functions (Ex.: `UI__Button("Button text", &CallbackFunction)`)
2) Call `UI__Draw()`

# Example:
```c
#include <stdio.h>

#include "external/raylib/raylib.h"

#include "UI.h"



void ExampleCallback() {
	puts("Click!");
}

char inputFieldData[64];

int main() {

	InitWindow(1280, 720, 0);

	while (!WindowShouldClose()) {

		BeginDrawing();

			UI__Label("EXAMPLE LABEL");
			UI__Gap();
			UI__Button("EXAMPLE BUTTON", &ExampleCallback);
			UI__InputField(inputFieldData, 64);

			UI__Draw();

		EndDrawing();

	}

	printf("\nInput field data: %s\n", inputFieldData);

	return 0;

}
```

### ^ Example output:

![](https://github.com/GeeTwentyFive/RaySimpleGUI/blob/main/docs/Example_Output.png)
