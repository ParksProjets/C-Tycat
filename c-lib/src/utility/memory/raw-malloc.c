/*

Raw malloc module: try finding malloc'ed size with glibc information.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/utility.h"
#include "private/config-values.h"


#if !_CTYCONF_ENABLE_MEMINFO

#include <stdint.h>


// Malloc alignement.
#define MALLOC_ALIGNMENT (2 * sizeof(size_t))

// Alignement mask.
#define MALLOC_ALIGN_MASK (MALLOC_ALIGNMENT - 1)


// Header (glibc compatible).
struct hdr {
    size_t size;
};


// Convert a ptr to an hdr.
#define ptr2hdr(mem) ((struct hdr *)((char*)(mem) - sizeof(struct hdr)))

// Get the malloc'ed size.
#define hdrsize(h) ((h)->size & ~(0b111))

// Is a ptr (not hdr) mis-aligned?
#define is_misaligned(ptr) (((uintptr_t)(ptr)) & MALLOC_ALIGN_MASK)



// Return 1 if ptr has been malloc'ed.
_Bool _ctycat_is_malloced(void *ptr)
{
    struct hdr *hdr = ptr2hdr(ptr);
    size_t size = hdrsize(hdr);

    if ((uintptr_t)hdr > (uintptr_t)-size || is_misaligned(ptr))
        return 0;

    // TODO: checks that previnuse of next chunk is 1.

    return 1;
}



// Get the malloc'ed array size.
size_t _ctycat_malloced_size(void *ptr)
{
    if (!_ctycat_is_malloced(ptr))
        return 0;

    return hdrsize(ptr2hdr(ptr));
}


#endif // !_CTYCONF_ENABLE_MEMINFO
