/*

Tycat a basic types.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include <stdio.h>
#include <string.h>
#include <ctycat.h>


struct Container {
    const char *string;
    char c;
    _Bool boolean;
    int integer;
    char **array;
    char fixed_str[20];
};


int main()
{
    double val = 42.1337;
    ctycat(val);

    char strz[] = "This is a string!";
    ctycat(strz);


    struct Container c = {
        .string = "Hello, my name is Guillaume.",
        .integer = 42,
        .boolean = 47,
        .fixed_str = "A short text"
    };

    struct Container *ptr = &c;

    printf("Size of structure : %lu\n", sizeof(struct Container));
    ctycat(ptr);

    ctycat(ptr->integer);
}
