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

#include <cstdint>

#define LIST_INSERT_BEFORE 0
#define LIST_INSERT_AFTER 1

template <typename T>
struct LL_NODE
{
    LL_NODE *prev;
    T data;
    LL_NODE *next;
};

// A Doubly Linked List Implementation
template <typename T>
class LIST
{
    public:
        void push(T Data)
        {
            // TODO: Implement default push
        }

        // Usable without needing the memory allocator to be started
        void push(LL_NODE<T> *node)
        {
            if (head == nullptr && tail == nullptr)
            {
                head = node;
                tail = head;
            }

            else
            {
                node->next = nullptr;
                node->prev = tail;
                tail->next = node;
                tail = node;    
            }
            size++;
        }

        T removeHead()
        {
            // TODO: Implement Popping
        }

        T first()
        {
            return head->data;
        }

        T last()
        {
            return tail->data;
        }

        LL_NODE<T> *getHead()
        {
            return head;
        }

        bool empty()
        {
            if (size == 0)
                return true;
            return false;
        }

        void insert(LL_NODE<T> *location, LL_NODE<T> *node, uint8_t insertLoc = LIST_INSERT_BEFORE)
        {
            if (insertLoc == LIST_INSERT_BEFORE)
            {
                node->prev = location->prev;
                node->next = location;

                if (location->prev != nullptr)
                {
                    location->prev->next = node;
                }

                location->prev = node;
            }

            else
            {
                node->next = location->next;
                node->prev = location;

                if (location->next != nullptr)
                {
                    location->next->prev = node;
                }

                location->next = node;
            }

            size++;
        }

        void remove(LL_NODE<T> *node)
        {
            if (head == node)
                head = node->next;

            if (tail == node)
                tail = node->next;

            if (node->prev != nullptr)
                node->prev->next = node->next;

            if (node->next != nullptr)
                node->next->prev = node->prev;

            node->next = nullptr;
            node->prev = nullptr;

            size--;
        }
    private:
        size_t size{0};
        LL_NODE<T> *head{nullptr};
        LL_NODE<T> *tail{nullptr};
};