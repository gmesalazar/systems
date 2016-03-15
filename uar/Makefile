CC      := gcc

FLAGS   := -Wall -g -O2

MAIN    := uar
LIST    := list

all: $(MAIN)

$(MAIN): $(LIST).o $(MAIN).o
	$(CC) $(FLAGS) $^ -lbsd -o $@

$(MAIN).o: $(MAIN).c
	$(CC) $(FLAGS) -c $<

$(LIST).o: $(LIST).c
	$(CC) $(FLAGS) -c $<

clean:
	rm -rf *.o $(MAIN)

test: myar
	@echo "\n---- Creating test files ----"
	echo "This" > f1
	echo "is a" > f2
	echo "test" > f3
	@echo "-----------------------------\n"
	./$(MAIN) r arch f1 f2 f3
	./$(MAIN) t arch
	rm f1
	./$(MAIN) x arch f1
	./$(MAIN) d arch f2
	./$(MAIN) t arch
	./$(MAIN) r arch f2

.PHONY: all clean test
