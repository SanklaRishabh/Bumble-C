INCDIR := ../include
OBJDIR := obj
LIBDIR := ../lib
CC := cc
CFLAGS := -Wall -Wextra -pedantic -I$(INCDIR)
_DEPS := builtins.h process.h
DEPS := $(patsubst %,$(INCDIR)/%,$(_DEPS))
_OBJ := main.o builtins.o process.o
OBJ := $(patsubst %,$(OBJDIR)/%,$(_OBJ))
TARGET := bmblsh

$(OBJDIR)/%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
