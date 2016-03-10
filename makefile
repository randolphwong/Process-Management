CC=gcc
CFLAGS=-I. -g -Wall -Werror -std=c89
LIBS = -pthread

ODIR = obj

DEPS = graph.h proc_management.h parser.h utility.h

_OBJ = main.o graph.o proc_management.o parser.o utility.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(OBJ): | $(ODIR)

$(ODIR):
	@mkdir -p $@

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm $(OBJ)
