/*

Manage unit tests.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "kvec.h"
#include "ctycat-test.h"


// An entry in the test vector.
typedef struct __TestEntry __TestEntry;
struct __TestEntry {
    const char *name;
    int count;
    __subtest *subtests;
};


// Is the test system initialized?
extern _Bool __tests_system_inited;

// All tests functions.
typedef kvec_t(__TestEntry) __tests_vec;
extern __tests_vec __tests;



// Sort all tests in lexicographical order.
void __sort_tests();


// Run some tests.
void __run_tests(const char *prefix);

// Run a specific test.
void __run_test(struct __TestEntry *entry);

// Run a specific sub-test.
void __run_subtest(__subtest func);
