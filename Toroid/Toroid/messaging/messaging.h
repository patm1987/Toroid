
#ifndef _MESSAGING_H
#define _MESSAGING_H

#include "messages.h"

typedef void(*Messaging_Listener)(message msg, void* pData);

void Messaging_Init();
void Messaging_Cleanup();

void Messaging_AddListener(Messaging_Listener listener, message msg);
void Messaging_RemoveListener(Messaging_Listener listener, message msg);

void Messaging_RaiseMessage(message msg, void* pData);

#endif//_MESSAGING_H
