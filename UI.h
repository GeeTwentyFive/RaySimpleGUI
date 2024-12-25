#ifndef _UI_H
#define _UI_H

#include "external/raylib/raylib.h"



#define _UI__MAX_WIDGETS 256



// Base (unscaled) height
const int _UI__HEIGHT = 14;

int _UI__scale;
int _UI__scrollOffset = 0;



typedef enum {

        _UI__WIDGETTYPE_GAP,
        _UI__WIDGETTYPE_LABEL,
        _UI__WIDGETTYPE_BUTTON,
        _UI__WIDGETTYPE_INPUTFIELD

} _UI__WidgetType;

typedef struct {

        _UI__WidgetType type;
        union {
                char *text;
                char *text_OUT;
        };
        union {
                void *callback;
                int text_OUT_size;
        };

} _UI__Widget;

_UI__Widget _UI__widgets[_UI__MAX_WIDGETS];
int _UI__widgets_size = 0;

void *_UI__persistentWidgetData[_UI__MAX_WIDGETS] = {};
int _UI__persistentWidgetData_index = 0;



void UI__Gap() {

        _UI__widgets[_UI__widgets_size] = (_UI__Widget) {
                .type = _UI__WIDGETTYPE_GAP
        };

        _UI__widgets_size++;

}



void UI__Label(char *text) {

        _UI__widgets[_UI__widgets_size] = (_UI__Widget) {
                .type = _UI__WIDGETTYPE_LABEL,
                .text = text
        };

        _UI__widgets_size++;

}



void UI__Button(char *text, void *callback) {

        _UI__widgets[_UI__widgets_size] = (_UI__Widget) {
                .type = _UI__WIDGETTYPE_BUTTON,
                .text = text,
                .callback = callback
        };

        _UI__widgets_size++;

}



void UI__InputField(char *text_OUT, int text_OUT_size) {

        _UI__widgets[_UI__widgets_size] = (_UI__Widget) {
                .type = _UI__WIDGETTYPE_INPUTFIELD,
                .text_OUT = text_OUT,
                .text_OUT_size = text_OUT_size
        };

        _UI__widgets_size++;

}







void _UI__DrawLabel(int verticalOffset, char *text);
void _UI__DrawButton(int verticalOffset, char *text, void *callback);
void _UI__DrawInputField(int verticalOffset, char *text_OUT, int text_OUT_size);

void UI__Draw() {

        ClearBackground(BLACK);



        // Scale widget size based on screen height
        _UI__scale = GetScreenHeight() / 180;



        // Center...

        int bCenterWidgets = 0;
        int totalHeight = _UI__widgets_size * _UI__HEIGHT*_UI__scale;

        if (totalHeight <= GetScreenHeight()) bCenterWidgets = 1;

        // ...or Scroll

        else {
                _UI__scrollOffset -= (int)(GetMouseWheelMove()*10 * _UI__scale);
                if (IsKeyDown(KEY_DOWN)) _UI__scrollOffset += _UI__scale;
                if (IsKeyDown(KEY_UP)) _UI__scrollOffset -= _UI__scale;

                if (_UI__scrollOffset > totalHeight-GetScreenHeight()) _UI__scrollOffset = totalHeight-GetScreenHeight();
                else if (_UI__scrollOffset < 0) _UI__scrollOffset = 0;
        }



        // Draw

        for (int i = 0; i <= _UI__widgets_size; i++) {

                int verticalOffset;
                if (bCenterWidgets) verticalOffset = (GetScreenHeight()/2 - totalHeight/2) + (_UI__HEIGHT*_UI__scale * i);
                else verticalOffset = (_UI__HEIGHT*_UI__scale * i) - _UI__scrollOffset;

                switch(_UI__widgets[i].type) {

                        case _UI__WIDGETTYPE_LABEL:
                                _UI__DrawLabel(verticalOffset, _UI__widgets[i].text);
                        break;

                        case _UI__WIDGETTYPE_BUTTON:
                                _UI__DrawButton(verticalOffset, _UI__widgets[i].text, _UI__widgets[i].callback);
                        break;

                        case _UI__WIDGETTYPE_INPUTFIELD:
                                _UI__DrawInputField(verticalOffset, _UI__widgets[i].text_OUT, _UI__widgets[i].text_OUT_size);
                        break;

                }

        }

        // Resets for next loop/frame;
        // So that library user can change composition of UI
        // by just calling the related UI__... functions
        // every frame.
        _UI__widgets_size = 0;
        _UI__persistentWidgetData_index = 0;

}







void _UI__DrawLabel(int verticalOffset, char *text) {

        int fontSize = _UI__HEIGHT*_UI__scale - 4;
        Vector2 textSize = MeasureTextEx(GetFontDefault(), text, fontSize, fontSize/10);
        DrawText(
                text,
                GetScreenWidth()/2 - textSize.x/2,
                verticalOffset + _UI__scale/2,
                fontSize,
                WHITE
        );

}



void _UI__DrawButton(int verticalOffset, char *text, void *callback) {

        Rectangle buttonRect = {
                .x = 0,
                .y = verticalOffset,
                .width = GetScreenWidth(),
                .height = _UI__HEIGHT*_UI__scale
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
                buttonRect.y + (buttonRect.height/2 - textSize.y/2) + _UI__scale/2,
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



void _UI__DrawInputField(int verticalOffset, char *text_OUT, int text_OUT_size) {

        int *bFocused = (int*)&_UI__persistentWidgetData[_UI__persistentWidgetData_index];
        _UI__persistentWidgetData_index++;



        Rectangle inputFieldRect = {
                .x = 0,
                .y = verticalOffset,
                .width = GetScreenWidth(),
                .height = _UI__HEIGHT*_UI__scale
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
                inputFieldRect.y + (inputFieldRect.height/2 - textSize.y/2) + _UI__scale/2,
                fontSize,
                WHITE
        );

        // Click check & handling

        if (
                IsMouseButtonReleased(MOUSE_BUTTON_LEFT) &&
                GetMouseY() >= inputFieldRect.y &&
                GetMouseY() <= inputFieldRect.y + inputFieldRect.height
        ) *bFocused = 1;
        else if (
                IsMouseButtonReleased(MOUSE_BUTTON_LEFT) ||
                IsKeyPressed(KEY_ESCAPE)
        ) *bFocused = 0;

        // Text input handling

        if (*bFocused) {
                if (IsKeyPressed(KEY_BACKSPACE) && TextLength(text_OUT) != 0) text_OUT[TextLength(text_OUT)-1] = 0;

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



#endif // _UI_H
