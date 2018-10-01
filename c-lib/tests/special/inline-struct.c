/*

Tycat a strcuture with an inline struct.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include <stdio.h>
#include <ctycat.h>


struct A {
    int number;
    struct {
        const char *key;
        int value;
        struct C *next;
    } data;
};

typedef struct {
    float real;
    char c;
} A_beautiful_name;

struct C {
    A_beautiful_name s;
    struct A *next;
};


int main()
{
    struct A a = {42, {"this is a key", 1337}};
    struct C c = {{78.3f, 'M'}};

    a.data.next = &c;
    c.next = &a;


    ctycat(a);
}
