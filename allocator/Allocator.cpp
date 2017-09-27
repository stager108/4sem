#include "Allocator.h"
#include "Heap.h"
#include "MainHeap.h"

Allocator::Allocator()
        :heaps(DEFAULT_HEAP_NUMBER),
         mainHeap(INITIAL_SUPERBLOCK_NUMBER)
{
    for(auto&& heap: heaps) {
        heap.setAllocator(this);
    }
    mainHeap.setAllocator(this);
}

void* Allocator::alloc(size_t size)
{
    if (size > SUPERBLOCK_SIZE / 2) {
        void* data = malloc(size + sizeof(void*));
        if(data == nullptr){
            return data;
        }
        *reinterpret_cast<Superblock**>(data) = nullptr;
        return (char*)data + sizeof(void*);
    }
    auto index = getHeapNumber();
    return heaps[index].allocate(size);
}

void Allocator::free(void* ptr)
{
    if(ptr == nullptr){
        return;
    }
    Superblock* ptrToOwner = (*reinterpret_cast<Superblock**>((char*)ptr - sizeof(void*)));
    if (ptrToOwner == nullptr) {
        std::free(ptrToOwner);
    } else {
        ptrToOwner->freeBlock(ptr);
        if (ptrToOwner->isTooEmpty()){
            ptrToOwner->getHeap()->cutSuperblock(ptrToOwner);
            mainHeap.insertSuperblock(ptrToOwner);
        }
    }
}

size_t Allocator::getHeapNumber()
{
    return std::hash<std::thread::id>()(std::this_thread::get_id()) % heaps.size();
}

MainHeap& Allocator::getMainHeap() { return mainHeap; }


Allocator allocator;
void* mtalloc(size_t size) { return allocator.alloc(size); }
void mtfree(void* ptr) { allocator.free(ptr); }
