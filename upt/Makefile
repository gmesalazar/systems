CC      := gcc
FLAGS   := -Wall -Wextra -g -O2
MAIN    := upt

all: $(MAIN)

$(MAIN): $(MAIN).c 
	$(CC) $(FLAGS) $< -o $@

clean:
	rm -rf $(MAIN)

test:
	./$(MAIN) .

.PHONY: all clean test
