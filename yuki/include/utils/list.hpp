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

#define LIST_INSERT_BEFORE 0
#define LIST_INSERT_AFTER 1

template <typename T>
struct LL_NODE
{
    LL_NODE *Prev;
    T Data;
    LL_NODE *Next;
};

// A Doubly Linked List Implementation
template <typename T>
class LIST
{
    public:
        VOID Push(T Data)
        {
            // TODO: Implement default push
        }

        // Usable without needing the memory allocator to be started
        VOID Push(LL_NODE<T> *Node)
        {
            if (Head == nullptr && Tail == nullptr)
            {
                Head = Node;
                Tail = Head;
            }

            else
            {
                Node->Next = nullptr;
                Node->Prev = Tail;
                Tail->Next = Node;
                Tail = Node;    
            }
            Size++;
        }

        T RemoveHead()
        {
            // TODO: Implement Popping
        }

        T First()
        {
            return Head->Data;
        }

        T Last()
        {
            return Tail->Data;
        }

        LL_NODE<T> *GetHead()
        {
            return Head;
        }

        BOOL Empty()
        {
            if (Size == 0)
                return true;
            return false;
        }

        VOID Insert(LL_NODE<T> *Location, LL_NODE<T> *Node, UINT8 InsertLoc = LIST_INSERT_BEFORE)
        {
            if (InsertLoc == LIST_INSERT_BEFORE)
            {
                Node->Prev = Location->Prev;
                Node->Next = Location;

                if (Location->Prev != nullptr)
                {
                    Location->Prev->Next = Node;
                }

                Location->Prev = Node;
            }

            else
            {
                Node->Next = Location->Next;
                Node->Prev = Location;

                if (Location->Next != nullptr)
                {
                    Location->Next->Prev = Node;
                }

                Location->Next = Node;
            }

            Size++;
        }

        VOID Remove(LL_NODE<T> *Node)
        {
            if (Head == Node)
                Head = Node->Next;

            if (Tail == Node)
                Tail = Node->Next;

            if (Node->Prev != nullptr)
                Node->Prev->Next = Node->Next;

            if (Node->Next != nullptr)
                Node->Next->Prev = Node->Prev;

            Node->Next = nullptr;
            Node->Prev = nullptr;

            Size--;
        }
    private:
        size_t Size{0};
        LL_NODE<T> *Head{nullptr};
        LL_NODE<T> *Tail{nullptr};
};