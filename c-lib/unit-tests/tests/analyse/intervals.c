/*

Test interval container.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include <ctycat-test.h>
#include <private/analyse.h>



// Test all basic feature of the interval container.
SUBTEST(test_basic_features)
{
    _ctycat_chunks = kh_init(_chunks);

    _ctycat_add_interval(10, 20);
    _ctycat_add_interval(25, 30);
    _ctycat_add_interval(40, 50);

    ASSERT(_ctycat_get_interval(15));

    kh_destroy(_chunks, _ctycat_chunks);
}



// Create the test, with all functions to run.
CREATE_TEST("analyse/intervals", {
    test_basic_features
});
