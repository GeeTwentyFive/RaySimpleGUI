// RaySimpleGUI - https://github.com/GeeTwentyFive/RaySimpleGUI
/*
MIT License

Copyright (c) 2024 Gee

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef _RaySimpleGUI_H
#define _RaySimpleGUI_H

#include "raylib/raylib.h"



#define _RaySimpleGUI__MAX_WIDGETS 256



// Base (unscaled) widget height
const int _RaySimpleGUI__WIDGET_HEIGHT = 14;

int _RaySimpleGUI__scale;
int _RaySimpleGUI__scrollOffset = 0;



typedef enum {

        _RaySimpleGUI__WIDGETTYPE_GAP,
        _RaySimpleGUI__WIDGETTYPE_LABEL,
        _RaySimpleGUI__WIDGETTYPE_BUTTON,
        _RaySimpleGUI__WIDGETTYPE_CHECKBOX,
        _RaySimpleGUI__WIDGETTYPE_INPUTFIELD

} _RaySimpleGUI__WidgetType;

typedef struct {

        _RaySimpleGUI__WidgetType type;
        union {
                char *text;
                char *text_OUT;
                int *bState;
        };
        union {
                void *callback;
                int text_OUT_size;
        };

} _RaySimpleGUI__Widget;

_RaySimpleGUI__Widget _RaySimpleGUI__widgets[_RaySimpleGUI__MAX_WIDGETS];
int _RaySimpleGUI__widgets_size = 0;

void *_RaySimpleGUI__persistentWidgetData[_RaySimpleGUI__MAX_WIDGETS] = {};
int _RaySimpleGUI__persistentWidgetData_index = 0;



void RaySimpleGUI__Gap() {

        _RaySimpleGUI__widgets[_RaySimpleGUI__widgets_size] = (_RaySimpleGUI__Widget) {
                .type = _RaySimpleGUI__WIDGETTYPE_GAP
        };

        _RaySimpleGUI__widgets_size++;

}



void RaySimpleGUI__Label(char *text) {

        _RaySimpleGUI__widgets[_RaySimpleGUI__widgets_size] = (_RaySimpleGUI__Widget) {
                .type = _RaySimpleGUI__WIDGETTYPE_LABEL,
                .text = text
        };

        _RaySimpleGUI__widgets_size++;

}



void RaySimpleGUI__Button(char *text, void *callback) {

        _RaySimpleGUI__widgets[_RaySimpleGUI__widgets_size] = (_RaySimpleGUI__Widget) {
                .type = _RaySimpleGUI__WIDGETTYPE_BUTTON,
                .text = text,
                .callback = callback
        };

        _RaySimpleGUI__widgets_size++;

}



void RaySimpleGUI__Checkbox(int *bState) {

        _RaySimpleGUI__widgets[_RaySimpleGUI__widgets_size] = (_RaySimpleGUI__Widget) {
                .type = _RaySimpleGUI__WIDGETTYPE_CHECKBOX,
                .bState = bState
        };

        _RaySimpleGUI__widgets_size++;

}



void RaySimpleGUI__InputField(char *text_OUT, int text_OUT_size) {

        _RaySimpleGUI__widgets[_RaySimpleGUI__widgets_size] = (_RaySimpleGUI__Widget) {
                .type = _RaySimpleGUI__WIDGETTYPE_INPUTFIELD,
                .text_OUT = text_OUT,
                .text_OUT_size = text_OUT_size
        };

        _RaySimpleGUI__widgets_size++;

}







void _RaySimpleGUI__DrawLabel(int verticalOffset, char *text);
void _RaySimpleGUI__DrawButton(int verticalOffset, char *text, void *callback);
void _RaySimpleGUI__DrawCheckbox(int verticalOffset, int *bState);
void _RaySimpleGUI__DrawInputField(int verticalOffset, char *text_OUT, int text_OUT_size);

void RaySimpleGUI__Draw() {

        ClearBackground(BLACK);



        // Scale widget size based on screen height
        _RaySimpleGUI__scale = GetScreenHeight() / 180;



        int bCenterWidgets = 0;
        int totalHeight = _RaySimpleGUI__widgets_size * _RaySimpleGUI__WIDGET_HEIGHT*_RaySimpleGUI__scale;

        // Center...

        if (totalHeight <= GetScreenHeight()) bCenterWidgets = 1;

        // ...or Scroll

        else {
                _RaySimpleGUI__scrollOffset -= (int)(GetMouseWheelMove()*10 * _RaySimpleGUI__scale);
                if (IsKeyDown(KEY_DOWN)) _RaySimpleGUI__scrollOffset += _RaySimpleGUI__scale;
                if (IsKeyDown(KEY_UP)) _RaySimpleGUI__scrollOffset -= _RaySimpleGUI__scale;

                if (_RaySimpleGUI__scrollOffset > totalHeight-GetScreenHeight()) _RaySimpleGUI__scrollOffset = totalHeight-GetScreenHeight();
                else if (_RaySimpleGUI__scrollOffset < 0) _RaySimpleGUI__scrollOffset = 0;
        }



        // Draw

        for (int i = 0; i < _RaySimpleGUI__widgets_size; i++) {

                int verticalOffset;
                if (bCenterWidgets) verticalOffset = (GetScreenHeight()/2 - totalHeight/2) + (_RaySimpleGUI__WIDGET_HEIGHT*_RaySimpleGUI__scale * i);
                else verticalOffset = (_RaySimpleGUI__WIDGET_HEIGHT*_RaySimpleGUI__scale * i) - _RaySimpleGUI__scrollOffset;

                switch(_RaySimpleGUI__widgets[i].type) {

                        case _RaySimpleGUI__WIDGETTYPE_LABEL:
                                _RaySimpleGUI__DrawLabel(verticalOffset, _RaySimpleGUI__widgets[i].text);
                        break;

                        case _RaySimpleGUI__WIDGETTYPE_BUTTON:
                                _RaySimpleGUI__DrawButton(verticalOffset, _RaySimpleGUI__widgets[i].text, _RaySimpleGUI__widgets[i].callback);
                        break;

                        case _RaySimpleGUI__WIDGETTYPE_CHECKBOX:
                                _RaySimpleGUI__DrawCheckbox(verticalOffset, _RaySimpleGUI__widgets[i].bState);
                        break;

                        case _RaySimpleGUI__WIDGETTYPE_INPUTFIELD:
                                _RaySimpleGUI__DrawInputField(verticalOffset, _RaySimpleGUI__widgets[i].text_OUT, _RaySimpleGUI__widgets[i].text_OUT_size);
                        break;

                }

        }

        // Resets for next loop/frame;
        // So that library user can change composition of UI
        // by just calling the related RaySimpleGUI__... functions
        // every frame.
        _RaySimpleGUI__widgets_size = 0;
        _RaySimpleGUI__persistentWidgetData_index = 0;

}







void _RaySimpleGUI__DrawLabel(int verticalOffset, char *text) {

        int fontSize = _RaySimpleGUI__WIDGET_HEIGHT*_RaySimpleGUI__scale - 4;
        Vector2 textSize = MeasureTextEx(GetFontDefault(), text, fontSize, fontSize/10);
        DrawText(
                text,
                GetScreenWidth()/2 - textSize.x/2,
                verticalOffset + _RaySimpleGUI__scale/2,
                fontSize,
                WHITE
        );

}



void _RaySimpleGUI__DrawButton(int verticalOffset, char *text, void *callback) {

        Rectangle buttonRect = {
                .x = 0,
                .y = verticalOffset,
                .width = GetScreenWidth(),
                .height = _RaySimpleGUI__WIDGET_HEIGHT*_RaySimpleGUI__scale
        };

        // Border

        DrawRectangle(
                buttonRect.x,
                buttonRect.y,
                buttonRect.width,
                buttonRect.height,
                WHITE
        );

        // Inner background

        DrawRectangle(
                buttonRect.x + 1,
                buttonRect.y + 1,
                buttonRect.width - 2,
                buttonRect.height - 2,
                BLACK
        );

        // Text (Centered)

        int fontSize = buttonRect.height - 4;
        Vector2 textSize = MeasureTextEx(GetFontDefault(), text, fontSize, fontSize/10);
        DrawText(
                text,
                GetScreenWidth()/2 - textSize.x/2,
                buttonRect.y + (buttonRect.height/2 - textSize.y/2) + _RaySimpleGUI__scale/2,
                fontSize,
                WHITE
        );

        // Click check & handling

        if (
                IsMouseButtonReleased(MOUSE_BUTTON_LEFT) &&
                GetMouseY() >= buttonRect.y &&
                GetMouseY() <= buttonRect.y + buttonRect.height
        ) ((void(*)())callback)();

}



void _RaySimpleGUI__DrawCheckbox(int verticalOffset, int *bState) {

        Rectangle checkboxRect = {
                .x = GetScreenWidth()/2 - (_RaySimpleGUI__WIDGET_HEIGHT*_RaySimpleGUI__scale)/2,
                .y = verticalOffset,
                .width = _RaySimpleGUI__WIDGET_HEIGHT*_RaySimpleGUI__scale,
                .height = _RaySimpleGUI__WIDGET_HEIGHT*_RaySimpleGUI__scale
        };

        // Background

        DrawRectangle(
                checkboxRect.x + 1,
                checkboxRect.y + 1,
                checkboxRect.width - 2,
                checkboxRect.height - 2,
                WHITE
        );

        // Checked? box

        DrawRectangle(
                checkboxRect.x + checkboxRect.width/4,
                checkboxRect.y + checkboxRect.height/4,
                checkboxRect.width - checkboxRect.width/2,
                checkboxRect.height - checkboxRect.height/2,
                *bState ? BLACK : WHITE
        );

        // Click check & handling

        if (
                IsMouseButtonReleased(MOUSE_BUTTON_LEFT) &&
                GetMouseY() >= checkboxRect.y &&
                GetMouseY() <= checkboxRect.y + checkboxRect.height &&
                GetMouseX() >= checkboxRect.x &&
                GetMouseX() <= checkboxRect.x + checkboxRect.width
        ) *bState = !*bState;

}



void _RaySimpleGUI__DrawInputField(int verticalOffset, char *text_OUT, int text_OUT_size) {

        int *bFocused = (int*)&(_RaySimpleGUI__persistentWidgetData[_RaySimpleGUI__persistentWidgetData_index]);
        _RaySimpleGUI__persistentWidgetData_index++;



        Rectangle inputFieldRect = {
                .x = 0,
                .y = verticalOffset,
                .width = GetScreenWidth(),
                .height = _RaySimpleGUI__WIDGET_HEIGHT*_RaySimpleGUI__scale
        };

        // Border

        DrawRectangle(
                inputFieldRect.x,
                inputFieldRect.y,
                inputFieldRect.width,
                inputFieldRect.height,
                WHITE
        );

        // Inner background

        DrawRectangle(
                inputFieldRect.x + 1,
                inputFieldRect.y + 1,
                inputFieldRect.width - 2,
                inputFieldRect.height - 2,
                *bFocused ? GRAY : DARKGRAY
        );

        // Text (Centered)

        int fontSize = inputFieldRect.height - 4;
        Vector2 textSize = MeasureTextEx(GetFontDefault(), text_OUT, fontSize, fontSize/10);
        DrawText(
                text_OUT,
                GetScreenWidth()/2 - textSize.x/2,
                inputFieldRect.y + (inputFieldRect.height/2 - textSize.y/2) + _RaySimpleGUI__scale/2,
                fontSize,
                WHITE
        );

        // Right click check & handling - paste from clipboard

        if (
                IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) &&
                GetMouseY() >= inputFieldRect.y &&
                GetMouseY() <= inputFieldRect.y + inputFieldRect.height
        ) {
                const char *clipboardText = GetClipboardText();
                if (TextLength(clipboardText)+1 < text_OUT_size) TextCopy(text_OUT, clipboardText);
        }

        // Left click check & handling - focus

        if (
                IsMouseButtonReleased(MOUSE_BUTTON_LEFT) &&
                GetMouseY() >= inputFieldRect.y &&
                GetMouseY() <= inputFieldRect.y + inputFieldRect.height
        ) *bFocused = 1;
        else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) *bFocused = 0;

        // Text input handling

        if (*bFocused) {
                if (IsKeyPressed(KEY_BACKSPACE) && TextLength(text_OUT) != 0) text_OUT[TextLength(text_OUT)-1] = 0;
                if (IsKeyPressed(KEY_ESCAPE)) text_OUT[0] = 0;

                for (
                        int inChar = GetCharPressed(); // Init
                        inChar != 0 && inChar < 128; // 0 == end of input queue, >128 == unicode
                        inChar = GetCharPressed()
                ) {
                        if (TextLength(text_OUT)+1 >= text_OUT_size) break;

                        text_OUT[TextLength(text_OUT)+1] = 0;
                        text_OUT[TextLength(text_OUT)] = (char)inChar;
                }
        }

}



#endif // _RaySimpleGUI_H
