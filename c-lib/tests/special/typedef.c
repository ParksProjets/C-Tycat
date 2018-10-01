/*

Tycat typedef types.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include <ctycat.h>


// Types aliases
typedef struct B my_struct;
typedef struct C * the_C_struct;
typedef const char * string;


// Structures
struct A {
    const my_struct *b;
    the_C_struct c;
};

struct B {
    string strz;
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

    a.b = &b;
    a.c = &c;

    b.strz = "This is a string.";
    b.num = 65;

    c.c = 'A';


    ctycat(a);
}
