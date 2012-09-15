
#include "keyboard.h"

#include "../messaging/messages.h"
#include "../messaging/messaging.h"

typedef struct KeyboardMessage_t
{
	KeyCode m_keyCode;
} KeyboardMessage;


KeyCode KeyboardMessage_GetCode(KeyboardMessage* pMessage)
{
	return pMessage->m_keyCode;
}

void Keyboard_Init()
{
	// nothing to do for Win32
}

void Keyboard_Cleanup()
{
	// nothing to do for Win32
}

void Keyboard_keyPressed(int32_t systemKeyCode)
{
	KeyboardMessage message = {Keyboard_systemToKeyCode(systemKeyCode)};
	Messaging_RaiseMessage(message_keyPressed, &message);
}

void Keyboard_keyReleased(int32_t systemKeyCode)
{
	KeyboardMessage message = {Keyboard_systemToKeyCode(systemKeyCode)};
	Messaging_RaiseMessage(message_keyReleased, &message);
}

KeyCode Keyboard_systemToKeyCode(int32_t systemKeyCode)
{
	return KeyCode_Unknown;
}

int32_t Keyboard_keyCodeToSystem(KeyCode keyCode)
{
	return 0;
}
