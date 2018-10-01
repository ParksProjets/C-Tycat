/*

Test "malloc-scan" feature.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include <ctycat.h>
#include <stdlib.h>


struct A {
    const char *name;
    int x;
    int y;
};


struct B {
    struct A base;
    const char *font;
    int fontsize;
};



// Create a "B" instance.
struct A *create_B()
{
    struct B *b = malloc(sizeof(struct B));

    b->base.name = "TextWidget";
    b->base.x = 12;
    b->base.y = 23;

    b->font = "Calibri";
    b->fontsize = 24;

    return (struct A *)b;
}



int main()
{
    struct A *a = create_B();

    ctycat(a);
}
