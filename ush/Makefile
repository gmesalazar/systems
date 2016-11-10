FLAGS   := -Wall -Wextra -g -O2 -std=c99

MAIN    := ush
LIST    := list
UTIL    := util

all: $(MAIN)

$(MAIN): $(UTIL).o $(LIST).o $(MAIN).o
	$(CC) $(FLAGS) *.o -o $(MAIN) -ledit

$(MAIN).o: $(MAIN).c
	$(CC) $(FLAGS) -I../lib -I/usr/local/include -c $<

$(UTIL).o: ../lib/$(UTIL).c
	$(CC) $(FLAGS) -c $<

$(LIST).o: $(LIST).c
	$(CC) $(FLAGS) -c $<

clean:
	rm -rf *.o $(MAIN)

.PHONY: all clean
