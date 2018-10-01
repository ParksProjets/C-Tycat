#
# Compile C-Tycat unit tests
#
# Copyright (C) 2018, Guillaume Gonnet
# This project is under the MIT License

CC = gcc
AR = ar rcs
CFLAGS = -g -std=c99 -Icore-include/ -I../include/
LDFLAGS := -L../ -lcty -ldw -lelf

EXE_NAME := test

OBJDIR  := obj
SRCDIR  := core-src
TESTDIR := tests


MKDIR = @mkdir -p $(@D)
RWCARD = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call RWCARD,$d/,$2))

SOURCES = $(call RWCARD,$(SRCDIR)/,*.c)
SOURCES += $(call RWCARD,$(TESTDIR)/,*.c)

OBJECTS_NAMES = $(patsubst %.c,%.o,$(SOURCES))
OBJECTS = $(addprefix $(OBJDIR)/,$(subst /,__,$(OBJECTS_NAMES)))

.SECONDEXPANSION:
.PHONY: clean


# Compile the static library
$(EXE_NAME): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $$(subst __,/,%).c
	$(MKDIR)
	$(CC) -o $@ -c $(CFLAGS) $<


# Clean generated objects
clean:
	rm -rf $(OBJDIR)
	rm -f $(EXE_NAME)
