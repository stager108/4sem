#include <iostream>
#include <stack>
#include <cstring>
//#include "Allocator.h"
#include "mtallocator.h"

const size_t N = 1000000;
//const size_t block_size = 100;
const size_t num_threads = 4;
const int block_size = 10;
/*
void TestSuperblock()
{
    Superblock superblock(128);
    std::vector<void*> allocated;
    for (size_t i = 0; i < SUPERBLOCK_SIZE/superblock.getBlockSize()-3; ++i) {
        allocated.push_back(superblock.getBlock());
    }
    for (auto& ptr: allocated) {
        superblock.freeBlock(ptr);
    }
    allocated.clear();
    for (size_t i = 0; i < SUPERBLOCK_SIZE/superblock.getBlockSize()-3; ++i) {
        allocated.push_back(superblock.getBlock());
    }
    for (auto& ptr: allocated) {
        superblock.freeBlock(ptr);
    }
}
*/
/*void TestHeap() {
    Heap heap;
    std::vector<void*> allocated;
    for (size_t i = 0; i < N; ++i) {
        allocated.push_back(heap.allocate(block_size));
    }
    for (auto& ptr: allocated) {
        (*reinterpret_cast<Superblock**>(ptr-sizeof(void*)))->freeBlock(ptr);
    }
    allocated.clear();
    for (size_t i = 0; i < N; ++i) {
        allocated.push_back(heap.allocate(block_size));
    }
    for (auto& ptr: allocated) {
        (*reinterpret_cast<Superblock**>(ptr-sizeof(void*)))->freeBlock(ptr);
    }

}*/

void TestAllocator(Allocator& allocator)
{
    std::vector<void*> allocated;
    for (size_t i = 0; i < N; ++i) {
        allocated.push_back(allocator.alloc(block_size));
        //memset(allocated.back(), 0, block_size-sizeof(void*));
    }
    for (auto& ptr: allocated) {
        //std::cout <<" free\n";
        allocator.free(ptr);
    }
    /*
    for (size_t i = 0; i < N; ++i) {
        allocated.push_back(allocator.alloc(block_size));
        //memset(allocated.back(), 0, block_size-sizeof(void*));
    }
    for (auto& ptr: allocated) {
        //std::cout <<" free\n";
        allocator.free(ptr);
    }*/
}

int main()
{
    //TestSuperblock();
    //TestHeap();
    Allocator allocator;
   // TestAllocator(allocator);
    std::vector<std::thread> threads;
    for (size_t i = 0; i<num_threads; ++i) {
        threads.emplace_back(&TestAllocator, std::ref(allocator));
    }
    for (auto&& thread:threads) {
        thread.join();
    }
    return 0;
}