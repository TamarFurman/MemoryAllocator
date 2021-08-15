//
// Created by bootcamp on 20/07/2021.
//



#include <glob.h>

#ifndef MEMORYALLOCATE_MEMORY_H
#define MEMORYALLOCATE_MEMORY_H

#endif //MEMORYALLOCATE_MEMORY_H
typedef struct MemoryAllocator {
    /* Memory block */
    char *memoryPool;
    size_t memorySize;
} MemoryAllocator;

/* memoryPool is a ptr to an already-existing large memory
block */

MemoryAllocator *MemoryAllocator_init(void *memoryPool,
                                      size_t size);

/* Returns a ptr to the memoryPool */
void *MemoryAllocator_release(MemoryAllocator *allocator);

void *MemoryAllocator_allocate(MemoryAllocator *allocator,
                               size_t size);

/* Merge the next adjacent block is free */
void MemoryAllocator_free(MemoryAllocator *allocator, void *
ptr);

/* Merges all adjacent free blocks, and returns the size of
largest free block */
size_t MemoryAllocator_optimize(MemoryAllocator *
allocator);