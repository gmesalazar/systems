CFLAGS   := -Wall -Wextra -g -O2 -std=c99 -I../include -I/usr/local/include

MAIN    := ush
LIST    := list
UTIL    := util

all: $(MAIN)

$(MAIN): $(UTIL).o $(LIST).o $(MAIN).o
	$(CC) $(CFLAGS) *.o -o $(MAIN) -ledit

$(MAIN).o: $(MAIN).c
	$(CC) $(CFLAGS) -c $<

$(UTIL).o: ../lib/$(UTIL).c
	$(CC) $(CFLAGS) -c $<

$(LIST).o: $(LIST).c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf *.o $(MAIN)

.PHONY: all clean
