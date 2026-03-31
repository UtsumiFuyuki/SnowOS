/**
Snow Operating System
Copyright (c) UtsumiFuyuki 2025, 2026
 
File: utils/list.hpp

Description:
structs and definitions for a basic linked list implementation
for Yuki

Author:
UtsumiFuyuki
March 29rd 2026
**/

#pragma once

#include <typedefs.hpp>

template <typename T>
struct LL_NODE
{
    LL_NODE *Prev;
    T Data;
    LL_NODE *Next;
};

// A Doubly Linked List Implementation
template <typename T>
class LINKED_LIST
{
    public:
        VOID Push(T Data)
        {
            // TODO: Implement default push
        }

        // Usable without needing the memory allocator to be started
        VOID Push(LL_NODE<T> *Node)
        {
            Node->Prev = nullptr;
            Node->Next = Head;

            if (Head != nullptr)
                Head->Prev = Node;
            Head = Node;
        }

        T RemoveHead()
        {
            // TODO: Implement Popping
        }
    private:
        ULONG64 Size{0};
        LL_NODE<T> *Head{nullptr};
};