CC        := gcc
CFLAGS    := -Wall -Wextra -I../util

MAIN      := ued
UTIL      := util

all:        $(MAIN)

$(MAIN):    $(MAIN).o $(UTIL).o
	        $(CC) $(CFLAGS) -o $@ $^

$(MAIN).o:  $(MAIN).c
	        $(CC) $(CFLAGS) -c $<

$(UTIL).o:  ../util/$(UTIL).c
	        $(CC) $(CFLAGS) -c $<

clean:
	        rm -rf *.o $(MAIN)

.PHONY: clean
