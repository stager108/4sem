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


const size_t SUPERBLOCK_SIZE = 4096;
const int DEFAULT_HEAP_NUMBER = (int)std::thread::hardware_concurrency()*2;

using byte = unsigned char;

class Heap;

int getDegree(int n){
    int ans = 1;
    int e = 1;
    while (2*e < n){
        ans++;
        e*=2;
    }
    return ans;
}

class Superblock {
    int block_size;
    int real_block_size;
    void* data;
    std::vector<void*> freeBlocks;
public:

    Superblock(int bsize, int place, int heap_number)
    {
        block_size = bsize;
        real_block_size = block_size+3 * sizeof(byte);
        data = malloc(SUPERBLOCK_SIZE);

        for (size_t i = 0; i < SUPERBLOCK_SIZE / real_block_size; i++) {
            auto address = (char*)data + (i * real_block_size);
            //*reinterpret_cast<Superblock**>(address) = this;
            ((byte*)address)[0] = (byte)heap_number;
            ((byte*)address)[1] = (byte)getDegree(block_size);
            ((byte*)address)[2] = (byte)place;
            freeBlocks.push_back(address);
        }
    }

    ~Superblock() { free(data);}

    bool hasFree(){ return !freeBlocks.empty(); }

    void* getBlock()
    {
        if (!freeBlocks.empty()) {
            auto next = freeBlocks.back();
            freeBlocks.pop_back();
            return (char*)next + 3*sizeof(byte);
        }
        assert(false);
    }

    void freeBlock(void* ptr){freeBlocks.push_back((char*)ptr - 3*sizeof(byte));}
};

class Heap {
public:
    std::vector<std::vector<Superblock*>> superblocks;
    std::vector<int> begins;

    std::mutex mut;
    Heap() {
        superblocks.resize(13);
        begins.resize(13, 0);
    }

    ~Heap()
    {
        for(int i = 0; i < 13; i++) {
            for (auto& superblock:superblocks[i]) {
                delete (superblock);
            }
        }
    }

    void* allocate(size_t size)
    {
        void* ans = nullptr;
        int s = getDegree((int)size);
        if (begins[s] < (int)superblocks[s].size() && superblocks[s][begins[s]]->hasFree()) {
            ans = superblocks[s][begins[s]]->getBlock();
        }
        while(begins[s] < (int)superblocks[s].size()&& !superblocks[s][begins[s]]->hasFree()){
            begins[s]++;
        }
        return ans;
    }
};

class Allocator {
public:
    Allocator():heaps(DEFAULT_HEAP_NUMBER){ }

    ~Allocator() { }

    void* alloc(size_t size)
    {
        if (size > SUPERBLOCK_SIZE / 2) {
            void* data = malloc(size + 3 * sizeof(byte));
            ((byte*)data)[0] = std::numeric_limits<byte>::max();
            return (char*)data + 3 * sizeof(byte);
        }
        auto index = getHeapNumber();
        auto bucket = &heaps[index];
        bucket->mut.lock();
        auto ans = bucket->allocate(size);
        bucket->mut.unlock();
        if(ans == nullptr){
            for(int j = 0; j < index; j++){
                heaps[j].mut.lock();
                ans = heaps[j].allocate(size);
                heaps[j].mut.unlock();
                if(ans != nullptr)
                    return ans;
            }
            for(int j = (int)index + 1; j < DEFAULT_HEAP_NUMBER; j++){
                heaps[j].mut.lock();
                ans = heaps[j].allocate(size);
                heaps[j].mut.unlock();
                if(ans != nullptr)
                    return ans;
            }
        }
        else{
            return ans;
        }
        bucket->mut.lock();
        ans = bucket->allocate(size);
        if(ans == nullptr) {
            int s = getDegree((int) size);
            Superblock* newS = new Superblock((int) size, (int) bucket->superblocks[s].size(), index);
            //std::cout << "a";
            ans = newS->getBlock();
            bucket->superblocks[s].push_back(newS);
        }
        bucket->mut.unlock();
        return ans;
    }

    void free(void* ptr)
    {
        if(ptr == nullptr)
            return;
        auto ptrToBeginning = (byte*)ptr - 3*sizeof(byte);

        if (((byte*)ptrToBeginning)[0] == std::numeric_limits<byte>::max()) {
            std::free(ptrToBeginning);
        } else {
            int heap_number = ptrToBeginning[0];
            int size = ptrToBeginning[1];
            int superblock_number = ptrToBeginning[2];
            auto ptrToOwner = heaps[heap_number].superblocks[size][superblock_number];
            heaps[heap_number].mut.lock();
            ptrToOwner->freeBlock(ptr);
            if(heaps[heap_number].begins[size] > superblock_number){
                heaps[heap_number].begins[size] = superblock_number;
            }
            heaps[heap_number].mut.unlock();
        }
    }

private:

    std::vector<Heap> heaps;

    int getHeapNumber()
    {
        return (int)std::hash<std::thread::id>()(std::this_thread::get_id()) % heaps.size();
    }
};

Allocator allocator;
void* mtalloc(size_t size) { return allocator.alloc(size); }
void mtfree(void* ptr) { allocator.free(ptr); }
