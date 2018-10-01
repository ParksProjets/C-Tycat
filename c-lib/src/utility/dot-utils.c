/*

Some useful functions for manipulating DOT files.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/utility.h"
#include "private/config.h"
#include "private/ctycat.h"
#include "private/dotify.h"

#include <string.h>
#include <unistd.h>



// Get the DOT name from the temp dir.
static const char *dot_temp()
{
    static char dotname[] = "/tmp/ctycatXXXXXX.dot";
    int fd = mkstemps(dotname, 4);

    f = fdopen(fd, "w");
    return dotname;
}



// Get the PNG name from the configuration.
static const char *dot_config()
{
    char *dot = strrchr(c.dot_name, '.');
    if (dot && !strcmp(dot, ".dot")) {
        f = fopen(c.dot_name, "w");
        return c.dot_name;
    }

    static char dotname[255];
    sprintf(dotname, "%s%d.dot", c.dot_name, _ctycat_number);

    f = fopen(dotname, "w");
    return dotname;
}



// Open the DOT file (and get its name).
const char *_ctycat_open_dot()
{
    if (c.save_dot && c.dot_name)
        return dot_config();
    else
        return dot_temp();
}




// Print and HTML escape a text.
void _ctycat_print_escaped(const char *text)
{
    for (; *text; text++) {
        switch (*text) {
            case '"':
                fputs("&quot;", f); break;
            case '&':
                fputs("&amp;", f); break;
            case '<':
                fputs("&lt;", f); break;
            case '>':
                fputs("&gt;", f); break;
            default:
                fputc(*text, f); break;
        }
    }
}
