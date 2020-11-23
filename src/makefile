IDIR =../include
CC=gcc
CFLAGS= -g -I $(IDIR)

ODIR=obj
LDIR =../lib

LIBS=-lm -lstdc++

_DEPS = leitordearquivos.hpp montador.hpp tradutor.hpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o leitordearquivos.o tradutor.o montador.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

tradutor: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 