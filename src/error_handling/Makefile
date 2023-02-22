# Makefile
# feito por Gabriel Pontarolo
CC = g++
TARGET = errorHandler

# flags
CPPFLAGS = -Wall -g --std=c++17
LDLIBS = -lm -lgmp

# diretorios fonte
VPATH = ../utils

objs = error_test.o convolution.o checksum.o crc.o bits.o trellis.o

.PHONY: all clean purge

all: $(TARGET)

# ligacao
$(TARGET): $(objs)
	$(CC) $(CPPFLAGS) $(LDLIBS) $(objs) -o $(TARGET)

# compilacao
error_test.o: errors.h
checksum.o: errors.h
crc.o: errors.h
convolution.o: errors.h bits.h
trellis.o: trellis.h bits.h 
bits.o: bits.h

# limpeza
clean:
	-rm -f $(objs) *~
purge: clean
	-rm -f $(TARGET)
