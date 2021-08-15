#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#include "memory.h"

int main() {
    void *array = (void *) malloc(1001 * sizeof(void));
    MemoryAllocator *memory = MemoryAllocator_init(array, 1000);
    int beforeAlloc = (int) MemoryAllocator_optimize(memory);
    printf("\nblock size: %d\n", beforeAlloc);
    char *a = MemoryAllocator_allocate(memory, 256);
    printf("\n%p\n", a);
    int afterfirstalloc = (int) MemoryAllocator_optimize(memory);
    printf("\nblock size: %d\n", afterfirstalloc);
    char *b = MemoryAllocator_allocate(memory, 5);
    printf("\n%p\n", b);
    int afterSecondAlloc = (int) MemoryAllocator_optimize(memory);
    printf("\nblock size: %d\n", afterSecondAlloc);
    MemoryAllocator_free(memory, b);
    printf("\nmemory pool:\n%p\n", memory->memoryPool);
    int afterFree = (int) MemoryAllocator_optimize(memory);
    printf("\nblock size: %d\n", afterFree);
    MemoryAllocator_release(memory);
    int emptyMemory = (int) MemoryAllocator_optimize(memory);
    printf("\nsize block after release:%d\n", emptyMemory);
    return 0;
}
