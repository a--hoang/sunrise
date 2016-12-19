########################################################################
# Code listing from "Advanced Linux Programming," by CodeSourcery LLC  #
# Copyright (C) 2001 by New Riders Publishing                          #
# See COPYRIGHT for license information.                               #
########################################################################


# C source files for the server.
SOURCES		= server.c common.c main.c arduino-serial-lib.c
# Object files corresponding to source files.
OBJECTS		= $(SOURCES:.c=.o)

# Default target: build everything.
all: server arduino

# Clean up build products.
clean:
	rm -f $(OBJECTS) server

# The main server program.  Link with -Wl,-export-dyanamic so
# dynamically loaded modules can bind symbols in the program.  Link in
# libdl, which contains calls for dynamic loading.
server: $(OBJECTS)
	$(CC) $(CFLAGS) -Wl,-export_dynamic -o $@ $^ -ldl

arduino: $(OBJECTS)


# All object files in the server depend on server.h.  But use the
# default rule for building object files from source files.
$(OBJECTS): server.h arduino-serial-lib.h
