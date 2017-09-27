#pragma once

#include <cstddef>
#include <vector>
#include <cassert>
#include <cstdlib>
#include "Config.h"

#include "Allocator.h"
#include "Heap.h"

class Superblock {
    size_t block_size;
    size_t real_block_size;
    void* data;
    std::vector<void*> freeBlocks;
    Heap* heap;

public:
    explicit Superblock(size_t bsize);

    ~Superblock();

    size_t getBlockSize();

    bool hasFree();

    void* getBlock();

    void freeBlock(void* ptr);

    bool isTooEmpty();

    size_t getFreeBlocksCount();

    Heap* getHeap();
    void setHeap(Heap* new_heap);
    void changeBlockSize(size_t newSize);

    bool isEmpty();
};