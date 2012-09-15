
#ifndef _MESSAGING_H
#define _MESSAGING_H

#include "messages.h"

/*!
 * \brief	listener for a message
 * \param	msg the message this listener is being raised with
 * \param	pUserData the user data associated with the event listener
 * \param	pData the event data sent with \a msg
 */
typedef void(*Messaging_Listener)(message msg, void* pUserData, void* pData);

void Messaging_Init();
void Messaging_Cleanup();

void Messaging_AddListener(Messaging_Listener listener, void* pUserData, message msg);
void Messaging_RemoveListener(Messaging_Listener listener, void* pUserData, message msg);

void Messaging_RaiseMessage(message msg, void* pData);

#endif//_MESSAGING_H
