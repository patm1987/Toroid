
#ifndef _LIST_H
#define _LIST_H

struct ListNode_t;
typedef struct ListNode_t ListNode;

struct List_t;
typedef struct List_t List;

//! @return	 0 if pLhs == pRhs, >0 if pLhs > pRhs, and <0 if pLhs < pRhs
typedef int(*ListComparator)(void* pLhs, void* pRhs, void* pUserData);
typedef void(*ForEachCallback)(void* pData, void* pUserData);
typedef void(*Destructor)(void* pData);

void* ListNode_getData(ListNode* pNode);
ListNode* ListNode_getNext(ListNode* pNode);
ListNode* ListNode_getPrevious(ListNode* pNode);
void* ListNode_remove(ListNode* pNode);

List* List_Create();
void List_Destroy(List* pList);
void List_DestroyWithDestructor(List* pList, Destructor destructor);

ListNode* List_getHead(List* pList);

void List_add(List* pList, void* pData);
void List_remove(List* pList, void* pData);
void List_removeCmp(List* pList, void* pData, ListComparator cmp, void* pUserData);

ListNode* List_Find(List* pList, void* pData, ListComparator cmp, void* pUserData);
void List_ForEach(List* pList, ForEachCallback cb, void* pUserData);

#endif//_LIST_H
