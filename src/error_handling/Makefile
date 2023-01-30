# Makefile
# feito por Gabriel Pontarolo
CC = g++
TARGET = errorHandler

# flags
CPPFLAGS = -Wall -g --std=c++17
LDLIBS = -lm -lgmp

# diretorios fonte
VPATH = ../utils

objs = main.o convolution.o checksum.o bits.o trellis.o

.PHONY: all clean purge

all: $(TARGET)

# ligacao
$(TARGET): $(objs)
	$(CC) $(CPPFLAGS) $(LDLIBS) $(objs) -o $(TARGET)

# compilacao
main.o: main.cpp checksum.h convolution.h
checksum.o: checksum.cpp checksum.h
convolution.o: convolution.cpp convolution.h bits.h
trellis.o: trellis.cpp trellis.h bits.h 
bits.o: bits.cpp bits.h

# limpeza
clean:
	-rm -f $(objs) *~
purge: clean
	-rm -f $(TARGET)

