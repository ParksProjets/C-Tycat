/*

Tycat an opaque type.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include <stdlib.h>
#include <ctycat.h>


// Structures
struct A {
    struct F *f;
    struct G **g;
};

struct B {
    const char *strz;
    int num;
};

struct C {
    char c;
};



int main()
{
    struct A a;
    struct B b;
    struct C c;

    a.f = (struct F *)&b;

    a.g = malloc(sizeof(struct G *));
    *a.g = (struct G *)&c;

    b.strz = "This is a string.";
    b.num = 65;

    c.c = 'A';


    ctycat(a);
}
