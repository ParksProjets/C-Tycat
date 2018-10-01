# C-Tycat C library

This folder contains the C library of **C-Tycat**.

Your project must be compiled with `-g` option for generating debug symbols.  
Beware that this library is **not thread safe**.



## Dependencies

**C-Tycat** needs **Libdw**, **LibElf** and **GraphViz**. Install the packages 
`libdw-dev` and `grahviz` to have them.



## Usage

There are few exported functions. These functions are prototyped in header
`ctycat.h`.  
Here are their description:

- `ctycat`: tycat a variable
- `ctycat_read_config`: read a configuration file (`.ini` like format)
- `ctycat_config_set`: set a configuration option (integer value)
- `ctycat_config_setstr`: read a configuration file (string value)



## Link C-Tycat in your executable

You can link **C-Tycat** with the following command:

```sh
-Wl,--whole-archive -lctys -Wl,--no-whole-archive -ldw -lelf
```

```sh
-include ctycat.h
```



## Troubleshooting

### I can't tycat a structure member

Currently, **C-Tycat** can only tycat a variable. Note that it's planned to
support tycating structure members.


### I can't tycat a rvalue

**C-Tycat** use the variable name to resolve data types. You can't tycat rvalues
because it's impossible to resolve types of rvalues.


### I have error "Can't find scopes containing ..."

**C-Tycat** doesn't work with position-independent code (because it cannot find
DWARF information from PC due to [Address space layout randomization][aslr]).  
Try recompiling your program with `-no-pie`.


### I have some multiple definitions when a compile

Make sure that the linking command is **before** any object files.



[aslr]: https://en.wikipedia.org/wiki/Address_space_layout_randomization
