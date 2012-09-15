
#include "messaging.h"

#include "../utils/list.h"

struct RaiseEventUserData_t
{
	message msg;
	void* pData;
};

struct MessageData_t
{
	Messaging_Listener listener;
	void* pUserData;
};

static List* s_listeners[message_count] = {0};

void RaiseEventForEachCB(void* pData, void* pUserData);

/*!
 * \brief	raises an event on each listener in a listener list
 * \param	pData a MessageData_t describing the listener/user data pair
 * \param	pUserData a RaiseEventUserData_t pair describing the event being raised and the
 *			data sent with that event
 */
void RaiseEventForEachCB(void* pData, void* pUserData)
{
	struct MessageData_t* messageData = (struct MessageData_t*)pData;
	struct RaiseEventUserData_t* pEventData = (struct RaiseEventUserData_t*)pUserData;
	messageData->listener(pEventData->msg, messageData->pUserData, pEventData->pData);
}

void Messaging_Init()
{
	// don't know if there's anything to do here... yet
}

void Messaging_Cleanup()
{
	// go over each event list and clean up the listeners
}

void Messaging_AddListener(Messaging_Listener listener, void* pUserData, message msg)
{
}

void Messaging_RemoveListener(Messaging_Listener listener, void* pUserData, message msg)
{
}

void Messaging_RaiseMessage(message msg, void* pData)
{
	struct RaiseEventUserData_t data = {msg, pData};
	List_ForEach(s_listeners[msg], RaiseEventForEachCB, &data);
}
