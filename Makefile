FNAME=hash_table
#LIBNAME=List_Funcs.cpp
CFLAGS= -c -Wall
LFLAGS= -o

all: $(FNAME)

$(FNAME): $(FNAME).o
	@g++ $(LFLAGS) $(FNAME) main.o
	@./$(FNAME)

$(FNAME).o:
	@g++ $(CFLAGS)  main.cpp



clean:
	@rm -rf $(FNAME) *.o