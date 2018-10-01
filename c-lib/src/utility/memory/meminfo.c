/*

Meminfo module: add some information in each malloc'ed chunk.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/analyse.h"
#include "private/config-values.h"


#if _CTYCONF_ENABLE_MEMINFO

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include <malloc.h>


// Old hook values.
static void (*old_free_hook) (void *, const void *);
static void *(*old_malloc_hook) (size_t, const void *);
static void *(*old_memalign_hook) (size_t, size_t, const void *);
static void *(*old_realloc_hook) (void *, size_t, const void *);


// Malloc alignement.
#define MALLOC_ALIGNMENT (2 * sizeof(size_t))

// Alignement mask.
#define MALLOC_ALIGN_MASK (MALLOC_ALIGNMENT - 1)



// Additional header.
struct hdr {
    size_t size;  // Exact size requested by user.
    unsigned long int magic;

#if _CTYCONF_SAFE_MEMORY_CHECK || _CTYCONF_ENABLE_MALLOC_SCAN
    unsigned long int address;
    unsigned long int magic3;
#endif
};


// Extra size to request to the malloc function.
#define EXTRA sizeof(struct hdr) + 1

// Magic numbers
#define MAGICWORD  0xA38FB83C
#define MAGICWORD2 0x56E1F19B
#define MAGICWORD3 0x3A6BE947
#define MAGICEND   ((char) 0x9A)


// Convert a ptr to an hdr.
#define ptr2hdr(mem) ((struct hdr *)((char *)(mem) - sizeof(struct hdr)))

// Is a ptr (not hdr) mis-aligned?
#define is_misaligned(ptr) (((uintptr_t)(ptr)) & MALLOC_ALIGN_MASK)




// Hook for the free function.
static void freehook(void *ptr, const void *caller)
{
    if (ptr != NULL)
        ptr = ptr2hdr(ptr);

    __free_hook = old_free_hook;

    if (old_free_hook != NULL)
        (*old_free_hook)(ptr, caller);
    else
        free(ptr);

    __free_hook = freehook;
}




// Hook for the memalign function.
static void *memalignhook(size_t alignment, size_t size, const void *caller)
{
    size_t slop = (sizeof(struct hdr) + alignment - 1) & -alignment;
    char *block;

    __memalign_hook = old_memalign_hook;

    if (old_memalign_hook != NULL)
        block = (*old_memalign_hook)(alignment, slop + size + 1, caller);
    else
        block = memalign(alignment, slop + size + 1);

    __memalign_hook = memalignhook;
    if (block == NULL) return NULL;

    struct hdr *hdr = ((struct hdr *)(block + slop)) - 1;

    hdr->magic = MAGICWORD;
    hdr->size = size;

#if _CTYCONF_ENABLE_MALLOC_SCAN
    hdr->address = _ctyext_find_address();
    hdr->magic3 = MAGICWORD3;
#elif _CTYCONF_SAFE_MEMORY_CHECK
    hdr->address = size ^ MAGICWORD2;
    hdr->magic3 = MAGICWORD3;
#endif

    *((char *)(hdr + 1) + size) = MAGICEND;
    return hdr + 1;
}




// Hook for the malloc function.
static void *mallochook(size_t size, const void *caller)
{
    struct hdr *hdr;
    __malloc_hook = old_malloc_hook;

    if (__builtin_expect(old_malloc_hook != NULL, 0))
        hdr = (*old_malloc_hook)(EXTRA + size, caller);
    else
        hdr = malloc(EXTRA + size);

    __malloc_hook = mallochook;
    if (hdr == NULL) return NULL;

    hdr->magic = MAGICWORD;
    hdr->size = size;

#if _CTYCONF_SAFE_MEMORY_CHECK
    hdr->address = size ^ MAGICWORD2;
    hdr->magic3 = MAGICWORD3;
#endif

    *((char *)(hdr + 1) + size) = MAGICEND;
    return hdr + 1;
}




// Hook for the realloc function.
static void *reallochook(void *ptr, size_t size, const void *caller)
{
    struct hdr *hdr;

    __malloc_hook = old_malloc_hook;
    __realloc_hook = old_realloc_hook;

    if (__builtin_expect(old_realloc_hook != NULL, 0))
        hdr = (*old_realloc_hook)(ptr, EXTRA + size, caller);
    else
        hdr = realloc(ptr, EXTRA + size);

    __malloc_hook = mallochook;
    __realloc_hook = reallochook;

    if (hdr == NULL) return NULL;

    hdr->size = size;
}




// Return 1 if ptr has been malloc'ed.
_Bool _ctycat_is_malloced(void *ptr)
{
    struct hdr *hdr = ptr2hdr(ptr);

    if ((uintptr_t)hdr > (uintptr_t)-hdr->size || is_misaligned(ptr))
        return 0;

#if _CTYCONF_SAFE_MEMORY_CHECK
    if (hdr->address ^ MAGICWORD2 != hdr->size || hdr->magic3 != MAGICWORD3)
        return 0;
#endif

    if (hdr->magic != MAGICWORD || *((char *)ptr + hdr->size) != MAGICEND)
        return 0;

    return 1;
}



// Get the malloc'ed array size.
size_t _ctycat_malloced_size(void *ptr)
{
    if (!_ctycat_is_malloced(ptr))
        return 0;

    return ptr2hdr(ptr)->size;
}




// Enable meminfo
// Note: priority is set to 101 to ensure that this function is the first called.
__attribute__((constructor(101))) static void enable()
{
    if (__free_hook == freehook)
        return;

    old_free_hook = __free_hook;
    __free_hook = freehook;

    old_malloc_hook = __malloc_hook;
    __malloc_hook = mallochook;

    old_memalign_hook = __memalign_hook;
    __memalign_hook = memalignhook;

    old_realloc_hook = __realloc_hook;
    __realloc_hook = reallochook;
}


#pragma GCC diagnostic pop

#endif // _CTYCONF_ENABLE_MEMINFO
