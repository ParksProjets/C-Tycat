/*

Tycat a strcuture with pointer to struct members.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include <stdio.h>
#include <ctycat.h>


#define ARRAY_SIZE 40

struct A {
    int first_mem;
    char c;
    struct B *ptr;
    int array[ARRAY_SIZE];
};

struct B {
    int *first_mem;
    int *from_array;
    char *c;
    struct A *ptr_to_A;
};


int main()
{
    struct A a = {42, 'G'};
    struct B b;

    a.ptr = &b;

    for (int i = 0; i < ARRAY_SIZE; i++)
        a.array[i] = 2 * i + 1;

    b.first_mem = &a.first_mem;
    b.from_array = &a.array[12];
    b.c = &a.c;
    b.ptr_to_A = &a;


    printf("Size of structure A : %lu\n", sizeof(struct A));
    printf("Size of structure B : %lu\n", sizeof(struct B));

    ctycat(a);
}
