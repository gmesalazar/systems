CFLAGS    := -Wall -Wextra -I../include

MAIN      := ued
UTIL      := util

all:        $(MAIN)

$(MAIN):    $(MAIN).o $(UTIL).o
	        $(CC) $(CFLAGS) $(.ALLSRC) -o $@

$(MAIN).o:  $(MAIN).c
	        $(CC) $(CFLAGS) -c $<

$(UTIL).o:  ../lib/$(UTIL).c
	        $(CC) $(CFLAGS) -c $<

clean:
	        rm -rf *.o $(MAIN)

.PHONY: clean
