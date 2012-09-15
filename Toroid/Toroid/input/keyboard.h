
#ifndef KEYBOARDMESSAGE_H
#define KEYBOARDMESSAGE_H

#include <stdint.h>

struct KeyboardMessage_t;
typedef struct KeyboardMessage_t KeyboardMessage;

typedef enum KeyCode_t
{
	KeyCode_Unknown,
	KeyCode_Tab,
	KeyCode_LeftShift,
	KeyCode_RightShift,
	KeyCode_LeftControl,
	KeyCode_RightControl,
	KeyCode_LeftSuper,
	KeyCode_RightSuper,
	KeyCode_LeftAlt,
	KeyCode_RightAlt,
	KeyCode_Enter,
	KeyCode_Space,
	KeyCode_Backspace,
	KeyCode_Delete,
	KeyCode_Tilde,
	KeyCode_Escape,
	KeyCode_Up,
	KeyCode_Down,
	KeyCode_Left,
	KeyCode_Right,
	KeyCode_Pause,
	KeyCode_PageDown,
	KeyCode_PageUp,
	KeyCode_A,
	KeyCode_B,
	KeyCode_C,
	KeyCode_D,
	KeyCode_E,
	KeyCode_F,
	KeyCode_G,
	KeyCode_H,
	KeyCode_J,
	KeyCode_K,
	KeyCode_L,
	KeyCode_M,
	KeyCode_N,
	KeyCode_O,
	KeyCode_P,
	KeyCode_Q,
	KeyCode_R,
	KeyCode_S,
	KeyCode_T,
	KeyCode_U,
	KeyCode_V,
	KeyCode_W,
	KeyCode_X,
	KeyCode_Y,
	KeyCode_Z,
	KeyCode_F1,
	KeyCode_F2,
	KeyCode_F3,
	KeyCode_F4,
	KeyCode_F5,
	KeyCode_F6,
	KeyCode_F7,
	KeyCode_F8,
	KeyCode_F9,
	KeyCode_F10,
	KeyCode_F11,
	KeyCode_F12,

	KeyCode_Count
} KeyCode;

KeyCode KeyboardMessage_GetCode(KeyboardMessage* pMessage);

void Keyboard_Init();
void Keyboard_Cleanup();
void Keyboard_keyPressed(int32_t systemKeyCode);
void Keyboard_keyReleased(int32_t systemKeyCode);
KeyCode Keyboard_systemToKeyCode(int32_t systemKeyCode);
int32_t Keyboard_keyCodeToSystem(KeyCode keyCode);

#endif//KEYBOARDMESSAGE_H
