CC      := gcc
FLAGS   := -Wall -g -O2

MAIN    := uws
UTIL    := util
LIBS    := -lpthread

all: $(MAIN)

$(MAIN): $(MAIN).o $(UTIL).o
	$(CC) $(FLAGS) $^ $(LIBS) -o $(MAIN)

$(MAIN).o: $(MAIN).c
	$(CC) $(FLAGS) -c $<

$(UTIL).o: $(UTIL).c
	$(CC) $(FLAGS) -c $<

clean:
	rm -rf *.o $(MAIN)
