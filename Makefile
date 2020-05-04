FNAME=hash_table
#LIBNAME=List_Funcs.cpp
CFLAGS= -c -Wall -msse4.2 -O0
LFLAGS= -o
ASM= strcmp

all: $(FNAME) clean

$(FNAME): $(FNAME).o $(ASM).o
	@g++ $(LFLAGS) $(FNAME) main.o $(ASM).o
	@./$(FNAME)

$(FNAME).o:
	@g++ $(CFLAGS)  main.cpp

$(ASM).o:
	@nasm -f elf64 $(ASM).s



clean:
	@rm -rf *.o