FNAME=test1
CFLAGS= -c -Wall
LFLAGS= -o

all: $(FNAME)

$(FNAME): $(FNAME).o
	@g++ $(LFLAGS) $(FNAME) main.o -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
	@./$(FNAME)

$(FNAME).o:
	@g++ $(CFLAGS)  main.cpp

clean:
	@rm -rf $(FNAME) *.o