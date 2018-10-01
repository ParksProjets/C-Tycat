/*

Some useful functons for printing node value. 

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/dotify.h"

#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <complex.h>


// Complex types
typedef complex double _cplxd;
typedef complex float _cplxf;



// Read a value from the memory.
static uintmax_t read_memory(void *ptr, int bytes)
{
    unsigned char *buffer = ptr;
    uintmax_t result = 0;

    for (int i = 0; i < bytes; ++i)
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        result |= buffer[i] << (i * CHAR_BIT);
#else
        result = (result << CHAR_BIT) | buffer[i];
#endif

    return result;
}




// Print a single character.
static void print_char(char c)
{
    if (isprint(c))
        fprintf(f, "%c", c);
    else
        fprintf(f, "\\x%x", c);
}



// Print a string.
static void print_string(const char *str)
{
    fputs("\"", f);

    for (; *str; str++)
        print_char(*str);

    fputs("\"", f);
}



// Print a string from a pointer (return true is the data is actually printed).
static _Bool print_ptr_string(_Cty_Die *fdie, void *data)
{
    Dwarf_Die die;
    Dwarf_Attribute attr;

    if (!data) return 0;

    dwarf_attr(&fdie->type, DW_AT_type, &attr);
    dwarf_formref_die(&attr, &die);

    int len = _ctydot_string_len(dwarf_dieoffset(&die), data);
    if (len == -1 || len > c.max_inline_string)
        return 0;

    print_string(data);
    return 1;
}



// Print a pointer address
static void print_pointer(_Cty_Die *die, void *data)
{
    void *pointed = *((void **)data);
    if (print_ptr_string(die, pointed)) return;

    if (pointed)
        fprintf(f, "%p", pointed);
    else
        fputs("NULL", f);
}



// Print a float.
static void print_float(int size, void *data)
{
    if (size == 8)
        fprintf(f, "%lf", *(double *)data);
    else
        fprintf(f, "%f", *(float *)data);
}



// Print a complex number.
static void print_complex(int size, void *data)
{
    if (size == 8)
        fprintf(f, "%lf%+lfi\n", creal(*(_cplxd *)data), cimag(*(_cplxd *)data));
    else
        fprintf(f, "%f%+fi\n", crealf(*(_cplxf *)data), cimagf(*(_cplxf *)data));
}




// Print a formeted value.
static void _ctydot_fvalue(_Cty_Die *die, void *data)
{
    Dwarf_Attribute attr;
    Dwarf_Word size, encoding;

    // Get the type of the die: is it a pointer?
    int type = dwarf_tag(&die->type);
    if (type == DW_TAG_pointer_type)
        return print_pointer(die, data);


    // Get the value from the memory
    dwarf_attr(&die->type, DW_AT_byte_size, &attr);
    dwarf_formudata(&attr, &size);

    // Print the value in the right form
    dwarf_attr(&die->type, DW_AT_encoding, &attr);
    dwarf_formudata(&attr, &encoding);


    switch (encoding) {
        case DW_ATE_address:
            fprintf(f, "%p", *(void **)data); break;
        case DW_ATE_boolean:
            fputs(*(_Bool *)data ? "true" : "false", f); break;
        case DW_ATE_signed:
            fprintf(f, "%jd", read_memory(data, size)); break;
        case DW_ATE_unsigned:
            fprintf(f, "%ju", read_memory(data, size)); break;
        case DW_ATE_signed_char:
            print_char(*(char *)data); break;
        case DW_ATE_float:
            print_float(size, data); break;
        case DW_ATE_complex_float:
            print_complex(size / 2, data); break;
        default:
            printf("Unknown ATE: %ld\n", encoding);
    }
}




// Print the value of a node.
void _ctydot_value(_Cty_Die *die, void *data)
{
    fputs("(<font color=\"royalblue\">", f);
    _ctydot_typename(die);

    fputs("</font>) ", f);
    _ctydot_fvalue(die, data);
}
