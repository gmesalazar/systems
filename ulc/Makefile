PROG      := ulc

PARSER    := $(PROG)_parser
SCANNER   := $(PROG)_scanner
CODEGEN   := $(PROG)_codegen
ENVIRON   := $(PROG)_environ
COMP      := $(PROG)_comp
VM        := $(PROG)_vm

ULC_C     := $(PROG)c
ULC_I     := $(PROG)i

UTIL      := util

OBJ       := $(PARSER) $(SCANNER) $(ENVIRON) $(CODEGEN) $(UTIL) $(VM)

CFLAGS    += -Wall -I../include -g

all: $(ULC_C) $(ULC_I) bin

bin:
	mkdir -p ./bin
	cp $(ULC_C) $(ULC_I) ./bin

$(ULC_C): $(COMP).c $(OBJ:=.o)
	$(CC) $(CFLAGS) -o $(ULC_C) $^ -lfl -lm

$(ULC_I): $(VM).c $(UTIL).o
	$(CC) $(CFLAGS) -o $(ULC_I) $^ -lm -DVM

$(PARSER).o:   $(PARSER).c
$(SCANNER).o:  $(SCANNER).c
$(ENVIRON).o: $(ENVIRON).c
$(CODEGEN).o:  $(CODEGEN).c
$(UTIL).o:     ../lib/$(UTIL).c
$(VM).o:       $(VM).c

$(OBJ:=.o):
	$(CC) $(CFLAGS) -c $<

$(PARSER).c: $(PARSER).y
	bison -d $(PARSER).y -o $@

$(SCANNER).c: $(SCANNER).l
	$(LEX) -o $(SCANNER).c $<

clean:
	rm -rf $(PARSER).c $(PARSER).h $(SCANNER).c $(ULC_I) $(ULC_C)
	rm -rf *.o
	rm -rf tests/*.ulb
	rm -rf tests/progs/*.ulb

.PHONY: clean
