/*

Create unit tests.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include <assert.h>


// Register a test.
typedef int (*__subtest)(void);
void __register_test(const char *name, __subtest tests[], int count);



// Create a new subtest.
#define SUBTEST(name) static int name(void)

// Create and register a new test.
#define CREATE_TEST(name, tests) \
    __attribute__((constructor)) static void __create_test() { \
        __register_test(name, (__subtest[])tests, \
            sizeof((__subtest[])tests) / sizeof(__subtest)); \
    }



// Assert that a value is true
#define ASSERT(value) assert(value)
