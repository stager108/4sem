#include "Heap.h"
#include "Superblock.h"
#include "Allocator.h"

class MainHeap : public Heap {
public:
    MainHeap(size_t initial_superblock_number);
private:
    using Heap::allocate;
    using Heap::cutSuperblock;
public:
    Superblock* getSuperblock(size_t block_size);
};
