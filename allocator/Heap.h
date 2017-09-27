#pragma once

#include <cstddef>
#include <mutex>
#include <list>
#include <cmath>
#include "Superblock.h"
#include "Allocator.h"
//#include "MainHeap.h"

class Heap {
protected:
    std::mutex mut;
    std::vector<Superblock*> superblocks;
    Allocator* allocator;
    //MainHeap* mainheap;

public:
    Heap() { }

    ~Heap();

    void* allocate(size_t size);
    void cutSuperblock(Superblock* superblock);

    void insertSuperblock(Superblock* superblock);

    void setAllocator(Allocator* mallocator);

};