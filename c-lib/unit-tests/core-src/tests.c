/*

Create unit tests.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "ctycat-test.h"
#include "core-private/tests.h"

#include <string.h>



// Is the test system initialized?
_Bool __tests_system_inited;

// All tests functions.
__tests_vec __tests;



// Compare two test entries.
static int comp_test(const void *p1, const void *p2)
{
    const __TestEntry *test1 = p1;
    const __TestEntry *test2 = p2;
    return strcmp(test1->name, test2->name);
}


// Sort all tests in lexicographical order.
void __sort_tests()
{
    __TestEntry *array = &kv_A(__tests, 0);
    qsort(array, kv_size(__tests), sizeof(__TestEntry), comp_test);
}



// Initialise the test system.
static void init_system()
{
    kv_init(__tests);

    __tests_system_inited = 1;
}


// Register a test.
void __register_test(const char *name, __subtest tests[], int count)
{
    if (!__tests_system_inited)
        init_system();

    __subtest *array = malloc(count * sizeof(__subtest));
    memcpy(array, tests, count * sizeof(__subtest));

    kv_push(__TestEntry, __tests, (__TestEntry){ name, count, array });
}
