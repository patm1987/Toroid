
#include "list.h"

#include <stdlib.h>
#include <string.h>

#include "memoryUtils.h"

// private interface
int MemoryAddressComparator(void* pLhs, void* pRhs, void* pUserData)
{
	return ((char*)pLhs) - ((char*)pRhs);
}
//

typedef struct ListNode_t
{
	ListNode* pNext;
	ListNode* pPrev;
	void* pData;
} ListNode;

typedef struct List_t
{
	ListNode* pHead;
} List;

void* ListNode_getData(ListNode* pNode)
{
	if (pNode) return pNode->pData;
	else return NULL;
}

ListNode* ListNode_getNext(ListNode* pNode)
{
	if (pNode) return pNode->pNext;
	else return NULL;
}

ListNode* ListNode_getPrevious(ListNode* pNode)
{
	if (pNode) return pNode->pPrev;
	else return NULL;
}

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

List* List_Create()
{
	List* pList = ALLOCATE(List);
	pList->pHead = NULL;
	return pList;
}

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

void List_DestroyWithDestructor(List* pList, Destructor destructor)
{
	ListNode* pIterator = pList->pHead;
	while(pIterator)
	{
		ListNode* pNext = ListNode_getNext(pIterator);
		destructor(ListNode_getData(pIterator));
		ListNode_remove(pIterator);
		pIterator = pNext;
	}
}

ListNode* List_getHead(List* pList)
{
	return pList->pHead;
}

void List_add(List* pList, void* pData)
{
	ListNode* pNode = ALLOCATE(ListNode);
	pNode->pData = pData;
	pNode->pNext = pList->pHead;
	pNode->pPrev = NULL;

	pList->pHead->pPrev = pNode;
	pList->pHead = pNode;
}

void List_remove(List* pList, void* pData)
{
	List_removeCmp(pList, pData, MemoryAddressComparator, NULL);
}

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
