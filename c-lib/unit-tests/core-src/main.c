/*

Run all unit tests.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "core-private/tests.h"
#include "core-private/logger.h"

#include <stdio.h>



// Print welcome message.
static void print_welcome()
{
    printf(KGRN
        "█▀▀   ▀█▀ █ █ █▀ █▀█ ▀█▀\n"
        "█      █   █  █  █▄█  █ \n"
        "█▄▄    █   █  █▄ █ █  █ \n\n"
           KYEL
        "Unit tests system.\n"
        "Released under the MIT License.\n"
    KNRM "\n");
}



// Run some tests.
static void run_tests(const char *prefix)
{
    int n = strlen(prefix);

    for (int i = 0; i < kv_size(__tests); i++) {
        if (strncmp(prefix, kv_A(__tests, i).name, n) == 0)
            __run_test(&kv_A(__tests, i));
    }
}



// Entry point of the application.
int main(int argc, char *argv[])
{
    print_welcome();

    __sort_tests();
    run_tests(argc == 1 ? "" : argv[1]);
}
