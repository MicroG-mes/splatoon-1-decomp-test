#pragma once

#include "types.h"

namespace sead {

template <typename T>
class TListNode {
public:
    TListNode* prev;
    TListNode* next;
    T* data;

    TListNode() : prev(nullptr), next(nullptr), data(nullptr) {}
};

template <typename T>
class TList {
public:
    TList() : mCount(0) {
        mRoot.next = &mRoot;
        mRoot.prev = &mRoot;
    }

    s32 size() const { return mCount; }
    bool empty() const { return mCount == 0; }

    void pushBack(TListNode<T>* node) {
        node->prev = mRoot.prev;
        node->next = &mRoot;
        mRoot.prev->next = node;
        mRoot.prev = node;
        mCount++;
    }

private:
    TListNode<T> mRoot;
    s32 mCount;
};

} // namespace sead
