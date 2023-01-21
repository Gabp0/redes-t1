# Makefile
# feito por Gabriel Pontarolo
CC = g++
TARGET = rawSocket

# flags
CPPFLAGS = -Wall -g
LDLIBS = -lm -lgmp

# diretorios fonte
VPATH = src:src/sockets

objs = main.o socket.o protocol.o checksum.o

.PHONY: all clean purge

all: $(TARGET)

# ligacao
$(TARGET): $(objs)
	$(CC) $(CPPFLAGS) $(LDLIBS) $(objs) -o socket

# compilacao
main.o: main.cpp 
socket.o: socket.c socket.h
protocol.o: protocol.cpp protocol.h
checksum.o: checksum.cpp checksum.h

# limpeza
clean:
	-rm -f $(objs) *~
purge: clean
	-rm -f rgps

