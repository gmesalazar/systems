PARSER		:= parser
SCANNER		:= scanner
SYMTABLE	:= symtable
CODEGEN		:= codegen
UTIL		:= util
VM		:= vm

OBJ		:= $(PARSER) $(SCANNER) $(SYMTABLE) $(CODEGEN) $(UTIL) $(VM)
COMP		:= comp

CCFLAGS		+= -Wall

all:		$(COMP) $(VM) bin

bin:
		mkdir -p ./bin
		cp $(COMP) $(VM) ./bin

debug:		CCFLAGS += -DYYDEBUG -g
debug:		$(COMP)

$(COMP):	$(COMP).c $(OBJ:=.o)
		$(CC) $(CCFLAGS) -o $(COMP) $^ -lfl -lm -lbsd

$(VM):		$(VM).c $(UTIL).o
		$(CC) -o $@ $^ -lm -DVM

$(PARSER).o:	$(PARSER).c
$(SCANNER).o:	$(SCANNER).c
$(SYMTABLE).o:	$(SYMTABLE).c
$(CODEGEN).o:	$(CODEGEN).c
$(UTIL).o:	$(UTIL).c
$(VM).o:	$(VM).c

$(OBJ:=.o):
		$(CC) $(CCFLAGS) -c $<

$(PARSER).c:	$(PARSER).y
		$(YACC) -do $(PARSER).c $<

$(SCANNER).c:	$(SCANNER).l
		$(LEX) -o $(SCANNER).c $<

clean:
		rm -rf $(PARSER).c $(PARSER).h $(SCANNER).c $(COMP) $(VM)
		rm -rf *.o
		rm -rf ./bin
		rm -rf test/*.b
		rm -rf test/progs/*.b

.PHONY: clean 
