# Makefile
# feito por Gabriel Pontarolo
CC = g++
TARGET = rawSocket

# flags
CPPFLAGS = -Wall -g --std=c++17
LDLIBS = -lm -lgmp

# diretorios fonte
VPATH = src:src/sockets:src/error_handling

objs = main.o socket.o githyanki.o checksum.o connection.o

.PHONY: all clean purge

all: $(TARGET)

# ligacao
$(TARGET): $(objs)
	$(CC) $(CPPFLAGS) $(LDLIBS) $(objs) -o socket

# compilacao
main.o: main.cpp githyanki.h
socket.o: socket.c socket.h
githyanki.o: githyanki.cpp githyanki.h
checksum.o: checksum.cpp checksum.h
connection.o: connection.cpp connection.h githyanki.h

# limpeza
clean:
	-rm -f $(objs) *~
purge: clean
	-rm -f $(TARGET)

