/*

Store hashed intervals in chunks.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/dotify.h"
#include "private/analyse.h"


// Hash map containing chunks
khash_t(_chunks) *_ctycat_chunks;

// Min/max functions
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))



// Get the interval containing an address.
struct _Interval *_ctycat_get_interval(uintptr_t addr)
{
    uintptr_t index = addr / c.chunk_size;

    khiter_t k = kh_get(_chunks, _ctycat_chunks, index);
    if (k == kh_end(_ctycat_chunks))
        return NULL;

    struct _Interval *interv = kh_value(_ctycat_chunks, k).top;

    for (; interv; interv = interv->next) {
        if (addr >= interv->start && addr <= interv->end)
            return interv;
    }

    return NULL;
}



// Add a new interval in a specific chunk.
static void chunk_add_interval(uintptr_t index, uintptr_t start, uintptr_t end)
{
    int exist;
    khiter_t k = kh_put(_chunks, _ctycat_chunks, index, &exist);
    struct _Chunck *chunck = &kh_value(_ctycat_chunks, k);

    // The chunck doesn't exist, we add the first interval in it.
    if (exist > 0) {
        chunck->top = malloc(sizeof(struct _Interval));
        *chunck->top = (struct _Interval){ NULL, 0, start, end };
    }


    struct _Interval *interv = chunck->top;
    while (interv->next && interv->next->end < start)
        interv = interv->next;

    // Merge overlapped intervals into a single one.
    if (interv->next && end >= interv->next->start) {
        interv = interv->next;
        interv->start = min(start, interv->start);

        struct _Interval *next, *cur = interv->next;
        for (; end >= cur->start; cur = next) {
            next = cur->next;
            free(cur);
        }

        interv->end = max(end, interv->end);
        interv->next = next;
    }

    // Add the interval between two (or at the end).
    else {
        struct _Interval *added = malloc(sizeof(struct _Interval));

        *added = (struct _Interval){ interv->next, 0, start, end };
        interv->next = added;
    }
}



// Add a new interval into the container.
void _ctycat_add_interval(uintptr_t start, uintptr_t end)
{
    uintptr_t starti = start / c.chunk_size;
    uintptr_t endi = end / c.chunk_size;

    if (starti == endi)
        return chunk_add_interval(starti, start, end);

    chunk_add_interval(starti, start, (starti + 1) * c.chunk_size);
    chunk_add_interval(endi, endi * c.chunk_size, end);

    for (starti++; starti < endi; starti++)
        chunk_add_interval(starti, starti * c.chunk_size, (starti + 1) * c.chunk_size);
}
