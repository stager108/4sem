#include <iostream>
#include <stack>
#include <cstring>
#include <cstddef>
#include <mutex>
#include <list>
#include <cmath>
#include <cstddef>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <thread>
#include <algorithm>


//const size_t N = 100000;
//const size_t block_size = 100;
//const size_t num_threads = 10;
const size_t PAGESIZE = 4096;
const size_t SUPERBLOCK_SIZE = PAGESIZE;
const double EMPTINESS_THRESHOLD = 0.5;
const size_t DEFAULT_HEAP_NUMBER = 4;
const size_t INITIAL_SUPERBLOCK_NUMBER = 20;

class Allocator;
class Heap;
class MainHeap;
class Allocator;

class Superblock {
    size_t block_size;
    size_t real_block_size;
    void* data;
    std::vector<void*> freeBlocks;
    Heap* heap;

public:
    explicit Superblock(size_t bsize)
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

    ~Superblock()
    {
        free(data);
    }

    size_t getBlockSize()
    {
        return block_size;
    }

    bool hasFree()
    {
        return !freeBlocks.empty();
    }

    void* getBlock()
    {
        if (!freeBlocks.empty()) {
            auto next = freeBlocks.back();
            freeBlocks.pop_back();
            return (char*)next + sizeof(void*);
        }
        assert(false);
    }

    void freeBlock(void* ptr)
    {
        freeBlocks.push_back((char*)ptr - sizeof(void*));
    }

    bool isTooEmpty(){
        return freeBlocks.size()*real_block_size < SUPERBLOCK_SIZE*EMPTINESS_THRESHOLD;
    }

    size_t getFreeBlocksCount() { return freeBlocks.size(); }

    Heap* getHeap() { return heap; }
    void setHeap(Heap* new_heap){
        heap = new_heap;
    }
};

class Heap {
protected:
    std::mutex mut;
    std::list<Superblock*> superblocks; // TODO: сортированный список
    Allocator* allocator;

public:
    Heap() { }

    ~Heap()
    {
        for (auto& superblock:superblocks) {
            delete (superblock);
        }
    }

    void* allocate(size_t size)
    {
        std::unique_lock<std::mutex> lock(mut);
        size_t s = (size_t) pow(2, (ceil(log2(size))));

        for (auto& superblock: superblocks) {
            if ((superblock->getBlockSize() >= s) && (superblock->hasFree())) {
                return superblock->getBlock();
            }
        }

        Superblock* newS = allocator->getMainHeap().getSuperblock(s);
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
        superblocks.push_back(newS);
        return ret;
    }
    void cutSuperblock(Superblock* superblock) {
        std::unique_lock<std::mutex> lock(mut);
        superblocks.erase(std::find(superblocks.begin(), superblocks.end(), superblock));
    }

    void insertSuperblock(Superblock* superblock) {
        for (auto i = superblocks.begin(); i != superblocks.end();) {
            if ((*i)->getBlockSize() < superblock->getBlockSize()) {
                i++;
            } else {
                superblocks.insert(i, superblock);
            }
        }
        superblock->setHeap(this);
    }

    void setAllocator(Allocator* mallocator) { allocator = mallocator; }

};


class MainHeap : public Heap {

public:
    MainHeap(size_t initial_superblock_number) {
        for(size_t i = 0; i< initial_superblock_number; ++i) {
            superblocks.push_back(new Superblock(0));
        }
    }
private:
    using Heap::allocate;
    using Heap::cutSuperblock;
public:
    Superblock* getSuperblock(size_t block_size) {

        //проверить, есть ли готовые

        Superblock* newS = new Superblock(block_size);
        return newS;
    }
};



class Allocator {
public:
    Allocator()
            :heaps(DEFAULT_HEAP_NUMBER),
             mainHeap(INITIAL_SUPERBLOCK_NUMBER)
    {
        for(auto&& heap: heaps) {
            heap.setAllocator(this);
        }
        mainHeap.setAllocator(this);
    }

    ~Allocator() { }

    void* alloc(size_t size)
    {
        if (size > SUPERBLOCK_SIZE / 2) {
            void* data = malloc(size + sizeof(void*));
            *reinterpret_cast<Superblock**>(data) = nullptr;
            return (char*)data + sizeof(void*);
        }
        auto index = getHeapNumber();
        return heaps[index].allocate(size);
    }

    void free(void* ptr)
    {
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

private:

    std::vector<Heap> heaps;
    MainHeap mainHeap;

    size_t getHeapNumber()
    {
        return std::hash<std::thread::id>()(std::this_thread::get_id()) % heaps.size();
    }

    MainHeap& getMainHeap() { return mainHeap; }

    friend class Heap;
    friend class MainHeap;
};

Allocator allocator;
void* mtalloc(size_t size) { return allocator.alloc(size); }
void mtfree(void* ptr) { allocator.free(ptr); }
