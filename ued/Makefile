CC        := gcc
CFLAGS    := -Wall -Wextra

MAIN      := ued
UTIL      := util

all:        $(MAIN)

$(MAIN):    $(MAIN).o $(UTIL).o
	        $(CC) $(CFLAGS) -o $@ $^

$(MAIN).o:  $(MAIN).c
	        $(CC) -c $<

$(UTIL).o:  ../util/$(UTIL).c
	        $(CC) -c $<

clean:
	        rm -rf *.o $(MAIN)

.PHONY: clean
