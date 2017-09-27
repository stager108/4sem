#include "Superblock.h"

Superblock::Superblock(size_t bsize)
{
    block_size = bsize;
    real_block_size = block_size+sizeof(void*);
    data = malloc(SUPERBLOCK_SIZE);

    for (size_t i = 0; i < SUPERBLOCK_SIZE / real_block_size; i++) {
        auto address = (char*)data + (i * real_block_size);
        *reinterpret_cast<Superblock**>(address) = this;
        freeBlocks.push_back(address);
    }
}

Superblock::~Superblock()
{
    //std::cout << getHeap()<< std::endl;
    free(data);
}

size_t Superblock::getBlockSize()
{
    return block_size;
}

bool Superblock::hasFree()
{
    return !freeBlocks.empty();
}

void* Superblock::getBlock()
{
    if (!freeBlocks.empty()) {
        auto next = freeBlocks.back();
        freeBlocks.pop_back();
        return (char*)next + sizeof(void*);
    }
    assert(false);
}

void Superblock::freeBlock(void* ptr)
{
    freeBlocks.push_back((char*)ptr - sizeof(void*));
}

bool Superblock::isTooEmpty()
{
    return freeBlocks.size()*real_block_size < SUPERBLOCK_SIZE*EMPTINESS_THRESHOLD;
}

size_t Superblock::getFreeBlocksCount() { return freeBlocks.size(); }

Heap* Superblock::getHeap() { return heap; }

void Superblock::setHeap(Heap* new_heap)
{
    heap = new_heap;
}

void Superblock::changeBlockSize(size_t newSize)
{
    block_size = newSize;
}

bool Superblock::isEmpty()
{
    return (freeBlocks.size() == (size_t)(SUPERBLOCK_SIZE/real_block_size));
}