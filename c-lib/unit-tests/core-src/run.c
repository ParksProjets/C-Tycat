/*

Run a test.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "ctycat-test.h"
#include "core-private/tests.h"

#include <string.h>


// Buffer for long-jumping into the error.
static sigjmp_buf buffer;


// Handle a system error.
static void handle_error(int sig)
{
    siglongjmp(buffer, 1);
}



// Run a specific sub-test.
static _Bool run_subtest(__subtest func)
{
    // if (sigsetjmp(buffer, 1))
    //     return 0;

    func();
}



// Run a specific test.
void __run_test(struct __TestEntry *entry)
{
    for (int i = 0; i < entry->count; i++)
        __run_subtest(entry->subtests[i]);
}
