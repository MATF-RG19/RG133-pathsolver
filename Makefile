PROGRAM   = pathsolver
CC        = gcc
CFLAGS    = -g -Wall
LDFLAGS   = -lGL -lGLU -lglut -lm

$(PROGRAM): main.o 
	$(CC) -o $(PROGRAM) main.c funkcije.c image.c $(LDFLAGS) 

.PHONY: clean dist

clean:
	-rm -f *.o $(PROGRAM) *core

dist: clean
	-tar -chvj -C .. -f ../$(PROGRAM).tar.bz2 $(PROGRAM)




run:
	./$(PROGRAM)
