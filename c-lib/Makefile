#
# Compile C-Tycat
#
# Copyright (C) 2018, Guillaume Gonnet
# This project is under the MIT License

CC = gcc
AR = ar rcs
CONFBUILDER = python3 builder/config/read-conf.py

CFLAGS = -std=gnu11 -Iinclude/
CTYCMD = -Wl,--whole-archive -lctys -Wl,--no-whole-archive -ldw -lelf

LIBFLAGS = -Iinclude/vendors -D_GNU_SOURCE -Wno-deprecated

LIB_STATIC  := libctys.a
LIB_DYNAMIC := libctyd.so

CONFIG := default-conf.ini
CONFVALUES := include/private/config-values.h

OBJDIR  := obj
SRCDIR  := src
TESTDIR := tests


MKDIR = @mkdir -p $(@D)
RWCARD = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call RWCARD,$d/,$2))

SOURCES = $(call RWCARD,$(SRCDIR)/,*.c)

OBJECTS_NAMES = $(patsubst $(SRCDIR)/%.c,%.o,$(SOURCES))
OBJECTS = $(addprefix $(OBJDIR)/,$(subst /,__,$(OBJECTS_NAMES)))

TESTS = $(subst /,.,$(patsubst $(TESTDIR)/%.c,%,$(call RWCARD,$(TESTDIR)/,*.c)))

.SECONDEXPANSION:
.PHONY: lib clean

# If a test name if provided: build this test
ifneq ($(filter $(MAKECMDGOALS),$(TESTS)),)
  TEST = $(MAKECMDGOALS)
  TESTOBJ = $(subst .,__,$(TEST))
endif



# Compile the library
lib: $(LIB_STATIC) $(LIB_DYNAMIC)

$(OBJDIR)/%.o: $(SRCDIR)/$$(subst __,/,%).c $(CONFVALUES)
	$(MKDIR)
	$(CC) -o $@ -fPIC -g -c $(CFLAGS) $(LIBFLAGS) $<

$(CONFVALUES): $(CONFIG)
	$(CONFBUILDER) -o $@ $<


# Compile the static library
$(LIB_STATIC): $(OBJECTS)
	$(AR) $@ $^

# Compile the dynamic library
$(LIB_DYNAMIC): $(OBJECTS)
	$(CC) -shared -o $@ -ldw -lelf $^



# Compile a test
$(TEST): $(OBJDIR)/tests/$(TESTOBJ).o $(LIB_STATIC)
	$(CC) -o $@ $(CTYCMD) $^ -lm

$(OBJDIR)/tests/%.o: $(TESTDIR)/$$(subst __,/,%).c
	$(MKDIR)
	$(CC) -o $@ -c -g $(CFLAGS) $<



# Clean generated objects
clean:
	rm -rf $(OBJDIR)
	rm -f $(LIB_STATIC) $(LIB_DYNAMIC) $(CONFVALUES) $(TESTS)
