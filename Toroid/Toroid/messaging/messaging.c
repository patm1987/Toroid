
#include "messaging.h"

#include <stdlib.h>

#include "../utils/list.h"
#include "../utils/memoryUtils.h"

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
void MessageDataDestructor(void* pData, void* pUserData);
int MessageDataComparator(void* pLhs, void* pRhs, void* pUserData);

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

/*!
 * \brief	used for cleaning up the listener list
 * \param	pData a MessageData_t to clean up
 * \param	pUserData - ignored
 */
void MessageDataDestructor(void* pData, void* pUserData)
{
	SAFE_FREE(pData);
}

int MessageDataComparator(void* pLhs, void* pRhs, void* pUserData)
{
	// TODO: compare pLhs and pRhs
}

void Messaging_Init()
{
	int index;
	for (index = 0; index < message_count; index++)
	{
		s_listeners[index] = List_Create();
	}
}

void Messaging_Cleanup()
{
	int index;
	for (index = 0; index < message_count; index++)
	{
		List_DestroyWithDestructor(s_listeners[index], MessageDataDestructor, NULL);
		s_listeners[index] = NULL;
	}
}

void Messaging_AddListener(Messaging_Listener listener, void* pUserData, message msg)
{
	struct MessageData_t *pMessageData = ALLOCATE(struct MessageData_t);
	List_add(s_listeners[msg], pMessageData);
}

void Messaging_RemoveListener(Messaging_Listener listener, void* pUserData, message msg)
{
	// TODO: ... yeah
}

void Messaging_RaiseMessage(message msg, void* pData)
{
	struct RaiseEventUserData_t data = {msg, pData};
	List_ForEach(s_listeners[msg], RaiseEventForEachCB, &data);
}
