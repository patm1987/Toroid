
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

/*!
 * \brief	comparator for MessageData
 * \param	pLhs a MessageData_t pointer for the left hand side of the comparison
 * \param	pRhs a MessageData_t pointer for the right hand side of the comparison
 * \param	pUserData - ignored
 * \return	0 if pLhs == pRhs, else return 1
 * \note	two MessageData_t's are equal iff their listener and user data portions both point
 *			to the same memory address
 */
int MessageDataComparator(void* pLhs, void* pRhs, void* pUserData)
{
	struct MessageData_t* pMsgLeft = (struct MessageData_t*)pLhs;
	struct MessageData_t* pMsgRight = (struct MessageData_t*)pRhs;

	if (pMsgLeft->listener == pMsgRight->listener
		&& pMsgLeft->pUserData == pMsgRight->pUserData)
	{
		return 0;
	}
	return 1;
}

/*!
 * \brief	initialize the Messaging system
 */
void Messaging_Init()
{
	int index;
	for (index = 0; index < message_count; index++)
	{
		s_listeners[index] = List_Create();
	}
}

/*!
 * \brief	cleanup the Messaging system
 */
void Messaging_Cleanup()
{
	int index;
	for (index = 0; index < message_count; index++)
	{
		List_DestroyWithDestructor(s_listeners[index], MessageDataDestructor, NULL);
		s_listeners[index] = NULL;
	}
}

/*!
 * \brief	adds a \a listener for the given message
 * \param	listener the callback to call when \a msg is raised
 * \param	pUserData user data sent to the \a listener 's pUserData parameter
 * \param	msg the message this \a listener will listen for
 */
void Messaging_AddListener(Messaging_Listener listener, void* pUserData, message msg)
{
	struct MessageData_t *pMessageData = ALLOCATE(struct MessageData_t);
	List_add(s_listeners[msg], pMessageData);
}

/*!
 * \brief	removes a \a listener for the given \a msg
 * \param	listener the callback for the listener that will be removed
 * \param	pUserData user data sent to the \a listener 's pUserData parameter, this MUST match
 *			the memory address of the original user data or the listener will NOT be removed
 * \param	msg the message the \a listener was listening for
 */
void Messaging_RemoveListener(Messaging_Listener listener, void* pUserData, message msg)
{
	struct MessageData_t msgData = {listener, pUserData};
	List_removeCmp(s_listeners[msg], &msgData, MessageDataComparator, NULL);
}

/*!
 * \brief	raises a message with associated user data
 * \param	msg the message to raise
 * \param	pData data sent to each listener's pData parameter
 */
void Messaging_RaiseMessage(message msg, void* pData)
{
	struct RaiseEventUserData_t data = {msg, pData};
	List_ForEach(s_listeners[msg], RaiseEventForEachCB, &data);
}
