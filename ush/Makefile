CC      := gcc

FLAGS   := -Wall -g -O2

MAIN    := ush
LIST    := list
UTIL    := util

all: $(MAIN)

$(MAIN): $(UTIL).o $(LIST).o $(MAIN).o
	$(CC) $(FLAGS) $^ -lbsd -o $(MAIN)

$(MAIN).o: $(MAIN).c
	$(CC) $(FLAGS) -c $<

$(UTIL).o: $(UTIL).c
	$(CC) $(FLAGS) -c $<

$(LIST).o: $(LIST).c
	$(CC) $(FLAGS) -c $<

clean:
	rm -rf *.o $(MAIN)

.PHONY: all clean
