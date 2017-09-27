#include "Heap.h"
#include <algorithm>

Heap::~Heap()
{
    for (auto& superblock:superblocks) {
        delete (superblock);
    }
}

void* Heap::allocate(size_t size)
{
    std::unique_lock<std::mutex> lock(mut);
    size_t s = (size_t) pow(2, (ceil(log2(size))));

    for (auto& superblock: superblocks) {
        if ((superblock->getBlockSize() >= s) && (superblock->hasFree())) {
            return superblock->getBlock();
        }
    }

    Superblock* newS = allocator->getMainHeap().getSuperblock(s);
    newS->setHeap(this);
    for (auto i = superblocks.begin(); i != superblocks.end();) {
        if ((*i)->getBlockSize() < s) {
            i++;
        } else {
            auto ret = newS->getBlock();
            superblocks.insert(i, newS);
            return ret;
        }
    }
    auto ret = newS->getBlock();
    if(!(newS->isTooEmpty())) {
        insertSuperblock(newS);
    }
    else{
        allocator->getMainHeap().insertSuperblock(newS);
    }
    return ret;
}

void Heap::cutSuperblock(Superblock* superblock)
{
    std::unique_lock<std::mutex> lock(mut);
    auto x = std::find(superblocks.begin(), superblocks.end(), superblock);
    if( x != superblocks.end()) {
        superblocks.erase(x);
    }
}

void Heap::insertSuperblock(Superblock* superblock)
{
    superblock->setHeap(this);

    for (auto i = superblocks.begin(); i != superblocks.end();) {
        if ((*i)->getBlockSize() < superblock->getBlockSize()) {
            i++;
        } else {
            superblocks.insert(i, superblock);
            return;
        }
    }
    if(superblock->isEmpty()){
        superblock->changeBlockSize(0);
    }
    superblocks.push_back(superblock);
}

void Heap::setAllocator(Allocator* mallocator) { allocator = mallocator; }
