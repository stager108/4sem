#include "MainHeap.h"

Superblock* MainHeap::getSuperblock(size_t block_size)
{
    if(superblocks.size() > 0){
        for(auto i = superblocks.begin(); i != superblocks.end();){
            if( (*i)->getBlockSize() == block_size){
                Superblock* newS = (*i);
                superblocks.erase(i);
                return newS;
            }
            else i++;
        }
        for(auto i = superblocks.begin(); i != superblocks.end();){
            if( (*i)->getBlockSize() == 0){
                Superblock* newS = (*i);
                superblocks.erase(i);
                newS->changeBlockSize(block_size);
                return newS;
            }
            else i++;
        }
    }
    Superblock* newS = new Superblock(block_size);
    return newS;
}

MainHeap::MainHeap(size_t initial_superblock_number)
{
    for(size_t i = 0; i< initial_superblock_number; ++i) {
        superblocks.push_back(new Superblock(0));
    }
}