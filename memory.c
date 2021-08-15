//
// Created by bootcamp on 20/07/2021.
//

#include "memory.h"
#include <stdio.h>
#include <stdlib.h>

#define NULL ((void *)0)
#define MASK 32767

void* release_array(void* ,int);
MemoryAllocator* MemoryAllocator_init(void* memoryPool,size_t size){
    char* memoryBlock = release_array(memoryPool,size);
    MemoryAllocator* myMemory = (MemoryAllocator*)malloc(sizeof(MemoryAllocator));
    myMemory ->memoryPool = memoryBlock;
    myMemory->memorySize= size;
    return myMemory;
}

void* MemoryAllocator_release(MemoryAllocator* allocator){
    return release_array(allocator->memoryPool,allocator->memorySize);
}

void* MemoryAllocator_allocate(MemoryAllocator* allocator,
                               size_t size){
    if(size > allocator->memorySize -2){
        return NULL;
    }
    char* memoryPool = allocator->memoryPool;
    int index = 0;
    while (index < allocator->memorySize){
        unsigned short needySize = size +2 + (8-(size +2)%8);
        short* tempShortPointer = (short*)(memoryPool+index);
        unsigned short sizeBlock = *tempShortPointer;
         if((sizeBlock >> 15) == 0){
            unsigned short withBit = needySize | 32768;
            if (sizeBlock >= needySize) {
                char *charIndex = (char *) &withBit;
                *(memoryPool + index) = *charIndex;
                *(memoryPool + index + 1) = *(charIndex + 1);
                if (sizeBlock > needySize) {
                    withBit = sizeBlock - needySize;
                    charIndex = (char *) &withBit;
                    *(memoryPool + index + needySize) = *charIndex;
                    *(memoryPool + index + needySize + 1) = *(charIndex + 1);
                }
                return memoryPool + index;
            }
        }else{
            index += (sizeBlock & MASK);
        }
    }
    return  NULL;
}


void MemoryAllocator_free(MemoryAllocator* allocator, void* ptr){
    short* tempShortPointer = (short*)(ptr);
    unsigned short blockSize = (*tempShortPointer & MASK);
    char* charPointer = (char*)ptr;
    for (int i = 2;i<blockSize; ++i) {
        *(charPointer + i) = 0;
    }
    short* pointerNextBlock = (short*)(charPointer + blockSize);
    short nextBlock = *pointerNextBlock;
    if (( nextBlock >> 15) == 0) {
        *(charPointer + nextBlock) = 0;
        *(charPointer + nextBlock+ 1) = 0;
        unsigned short newSize = nextBlock + blockSize;
        char* sizePointer = (char*)&newSize;
        *(charPointer) = *sizePointer;
        *(charPointer + 1) = *(sizePointer + 1);
    }
    else{
        char* sizePointer = (char*)&blockSize;
        *(charPointer) = *sizePointer;
        *(charPointer + 1) = *(sizePointer + 1);
    }
}

size_t MemoryAllocator_optimize(MemoryAllocator*
allocator){
    char* memoryPool = allocator->memoryPool;
    int index = 0;
    unsigned short  maxSize = 0;
    while (index < allocator->memorySize){
        short* tempShortPointer = (short*)(memoryPool+index);
        unsigned short sizeBlock = *tempShortPointer;
        unsigned short sizeFreeBit = sizeBlock & MASK;
        if((sizeBlock >> 15) == 0){
            if(sizeFreeBit >maxSize){
                maxSize = sizeFreeBit;
            }
            short* tempNextBlock = (short*)(memoryPool+sizeFreeBit);
            unsigned short sizeNextBlock = *tempNextBlock;
            if ((sizeNextBlock >> 15) == 0) {
                *(memoryPool + index+ sizeFreeBit) = 0;
                *(memoryPool + index + sizeFreeBit+ 1) = 0;
                unsigned short newSize = sizeFreeBit + (sizeNextBlock & MASK);
                char* sizePointer = (char*)&newSize;
                *(memoryPool + index) = *sizePointer;
                *(memoryPool + index + 1) = *(sizePointer + 1);
                index += sizeFreeBit;
            }else{
                index += sizeFreeBit + (sizeNextBlock&MASK);
            }
        }else{
            index += sizeFreeBit;
        }
    }
    return  maxSize;

}


void* release_array(void* memoryPool,int size){
    char* memoryBlock = (char*)malloc(size * sizeof(char));
     memoryBlock = (char*)memoryPool;
     for(int i = 2; i<size ;i++){
         memoryBlock[i] = 0;
     }
    char* tempChar = (char*)&size;
    memoryBlock[0] = *tempChar;
    memoryBlock[1] = *(tempChar+1);
    return memoryBlock;
}