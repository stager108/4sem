#pragma once

#include <cstddef>
#include <cstdlib>
#include <thread>
#include "Heap.h"
#include "MainHeap.h"
#include "Superblock.h"
#include "Config.h"

class Allocator {
public:
    Allocator();

    ~Allocator() { }

    void* alloc(size_t size);

    void free(void* ptr);

private:

    std::vector<Heap> heaps;
    MainHeap mainHeap;

    size_t getHeapNumber();

    MainHeap& getMainHeap();

    friend class Heap;
    friend class MainHeap;
};
