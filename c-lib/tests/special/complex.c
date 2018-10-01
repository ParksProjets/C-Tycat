/*

Tycat a complex number.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include <stdio.h>
#include <ctycat.h>
#include <math.h>
#include <complex.h>


struct MyStruct {
    double complex z1;
    double complex z2;
    double complex z3;
    float complex number;
};


int main()
{
    struct MyStruct my_variable;

    my_variable.z1 = 2.12 + 4.587 * I;
    my_variable.z2 = csqrt(-1);

    double PI = acos(-1);
    my_variable.z3 = cexp(I * PI);

    my_variable.number = 45 - 3 * I;

    ctycat(my_variable);
}
