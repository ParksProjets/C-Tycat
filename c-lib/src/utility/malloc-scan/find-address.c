/*

Do a backtrace and find address of the function that called malloc.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/utility.h"

#include <execinfo.h>



// Do a backtrace and find address of the function that called malloc.
size_t _ctyext_find_address()
{
    void *addresses[5];
    int count = backtrace(addresses, 5);


}
