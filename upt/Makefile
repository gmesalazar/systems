CC      := gcc
FLAGS   := -Wall -g -O2
MAIN    := pt

all: $(MAIN)

$(MAIN): $(MAIN).c 
	$(CC) $(FLAGS) $< -o $@

clean:
	rm -rf $(MAIN)

test:
	./$(MAIN) .

.PHONY: all clean test
