
#include "list.h"

#include <stdlib.h>
#include <string.h>

#include "memoryUtils.h"

// private interface

/*!
 * \brief	Comparison callback by memory address (default comparator)
 * \param	pLhs the left hand side of the comparison
 * \param	pRhs the right hand side of the comparison
 * \param	pUserData ignored
 * \return	0 if pLhs == pRhs
 *			>0 if pLhs > pRhs
 *			<0 if pLhs < pRhs
 */
int MemoryAddressComparator(void* pLhs, void* pRhs, void* pUserData)
{
	return ((char*)pLhs) - ((char*)pRhs);
}
//

//! @brief	a node in the list
typedef struct ListNode_t
{
	ListNode* pNext;	//!< \brief	the next node in the list (NULL if this is the end)
	ListNode* pPrev;	//!< \brief	the previous node in the list (NULL if this is the first)
	void* pData;		//!< \brief	data stored in this node
} ListNode;

//! \brief	a list
typedef struct List_t
{
	ListNode* pHead;	//!< \brief	the head of this list
} List;

/*!
 * \param	pNode the node to retrieve data from
 * \return	the data stored in \a pNode
 */
void* ListNode_getData(ListNode* pNode)
{
	if (pNode) return pNode->pData;
	else return NULL;
}

/*!
 * \param	pNode the current node
 * \return	the node after \a pNode
 */
ListNode* ListNode_getNext(ListNode* pNode)
{
	if (pNode) return pNode->pNext;
	else return NULL;
}

/*!
 * \param	pNode the current node
 * \return	the node before \a pNode
 */
ListNode* ListNode_getPrevious(ListNode* pNode)
{
	if (pNode) return pNode->pPrev;
	else return NULL;
}

/*!
 * \brief	removes \a pNode from its list, returning \a pNode's user data
 * \param	pNode the node to remove
 * \return	\a pNode's user data
 */
void* ListNode_remove(ListNode* pNode)
{
	void* pData;
	if (!pNode) return NULL;

	pData = ListNode_getData(pNode);
	if (pNode->pPrev)
	{
		pNode->pPrev->pNext = pNode->pNext;
	}
	if (pNode->pNext)
	{
		pNode->pNext->pPrev = pNode->pPrev;
	}
	SAFE_FREE(pNode);
	return pData;
}

/*!
 * \brief	creates a new list
 * \return	the new list
 */
List* List_Create()
{
	List* pList = ALLOCATE(List);
	pList->pHead = NULL;
	return pList;
}

/*!
 * \brief	destroys a list created with List_Create
 * \param	pList the list to destroy
 * \note	data elements in any remaining nodes will NOT be cleaned up, see
 *			List_DestroyWithDestructor if this is not desired
 */
void List_Destroy(List* pList)
{
	ListNode* pIterator = pList->pHead;
	while(pIterator)
	{
		ListNode* pNext = ListNode_getNext(pIterator);
		ListNode_remove(pIterator);
		pIterator = pNext;
	}
}

/*!
 * \brief	destroys a list created with List_Create, calling the \a destructor on each user
 *			data portion of a list node before cleaning up each node
 * \param	pList the list to destroy
 * \param	destructor the destructor to call on each list node's data element
 * \param	pUserData user data to pass to \a destructor
 */
void List_DestroyWithDestructor(List* pList, Destructor destructor, void* pUserData)
{
	ListNode* pIterator = pList->pHead;
	while(pIterator)
	{
		ListNode* pNext = ListNode_getNext(pIterator);
		destructor(ListNode_getData(pIterator), pUserData);
		ListNode_remove(pIterator);
		pIterator = pNext;
	}
}

/*!
 * \param	pList the list to retrieve the head element from
 * \return	the head of \a pList
 */
ListNode* List_getHead(List* pList)
{
	return pList->pHead;
}

/*!
 * \brief	adds a node to the beginning of the list
 * \param	pList the list to add a node to
 * \param	pData the data to insert into \a pList
 */
void List_add(List* pList, void* pData)
{
	ListNode* pNode = ALLOCATE(ListNode);
	pNode->pData = pData;
	pNode->pNext = pList->pHead;
	pNode->pPrev = NULL;

	pList->pHead->pPrev = pNode;
	pList->pHead = pNode;
}

/*!
 * \brief	removes a node containing \a pData from \a pList
 * \param	pList the list to remove \a pData from
 * \param	pData the data to remove (by memory address)
 */
void List_remove(List* pList, void* pData)
{
	List_removeCmp(pList, pData, MemoryAddressComparator, NULL);
}

/*!
 * \brief	removes \a pData from \a pList where equality is determined via the provided
 *			comparator rather than memory address
 * \param	pList the list to remove from
 * \param	pData the data to remove from the list
 * \param	cmp the comparator describing equality for \a pData
 * \param	pUserData the user data to pass to \a cmp when it's called
 */
void List_removeCmp(List* pList, void* pData, ListComparator cmp, void* pUserData)
{
	ListNode* pIterator = pList->pHead;
	while(pIterator)
	{
		void* pTestData = ListNode_getData(pIterator);
		if (cmp(pTestData, pData, pUserData) == 0)
		{
			ListNode_remove(pIterator);
			return;
		}
	}
	return;
}

/*!
 * \brief	finds the first instance of \a pData in \a pList where equality is defined by the
 *			provided comparator
 * \param	pList the list to find \a pData in
 * \param	pData the data to find
 * \param	cmp the comparator describing equality for \a pData
 * \param	pUserData user data to be passed to \a cmp
 */
ListNode* List_Find(List* pList, void* pData, ListComparator cmp, void* pUserData)
{
	ListNode* pIterator = pList->pHead;
	while(pIterator)
	{
		if (cmp(pData, pIterator->pData, pUserData) == 0)
		{
			return pIterator;
		}
		pIterator = pIterator->pNext;
	}
	return NULL;
}

/*!
 * \brief	foreach loop on \a pList
 * \param	pList the list to iterate over
 * \param	cb the callback to call on each element of \a pList
 * \param	pUserData user data to pass to \a cb for each list element in \a pList
 */
void List_ForEach(List* pList, ForEachCallback cb, void* pUserData)
{
	ListNode* pIterator = pList->pHead;
	while(pIterator)
	{
		ListNode* pCurrent = pIterator;
		pIterator = pIterator->pNext;

		cb(pIterator, pUserData);
	}
}
