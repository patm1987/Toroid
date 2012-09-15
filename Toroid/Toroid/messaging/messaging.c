
#include "messaging.h"

#include "../utils/list.h"

struct RaiseEventOutData
{
	message msg;
	void* pData;
};

static List* s_listeners[message_count] = {0};

void RaiseEventForEachCB(void* pData, void* pUserData);

void RaiseEventForEachCB(void* pData, void* pUserData)
{
	Messaging_Listener listener = (Messaging_Listener)pData;
	struct RaiseEventOutData* pEventData = (struct RaiseEventOutData*)pUserData;
	listener(pEventData->msg, pEventData->pData);
}

void Messaging_Init()
{
}

void Messaging_Cleanup()
{
}

void Messaging_AddListener(Messaging_Listener listener, message msg)
{
}

void Messaging_RemoveListener(Messaging_Listener listener, message msg)
{
}

void Messaging_RaiseMessage(message msg, void* pData)
{
	struct RaiseEventOutData data = {msg, pData};
	List_ForEach(s_listeners[msg], RaiseEventForEachCB, &data);
}
