# Makefile
# feito por Gabriel Pontarolo
CC = g++
TARGET = rawSocket

# flags
CPPFLAGS = -Wall -g
LDLIBS = -lm -lgmp

# diretorios fonte
VPATH = src:src/sockets

objs = main.o githyanki.o connection.o common.o socket.o

.PHONY: all clean purge

all: $(TARGET)

# ligacao
$(TARGET): $(objs)
	$(CC) $(CPPFLAGS) $(LDLIBS) $(objs) -o socket

# compilacao
main.o: main.cpp connection.h githyanki.h common.h
socket.o: socket.c socket.h
common.o: common.cpp common.h
githyanki.o: githyanki.cpp githyanki.h connection.h common.h
connection.o: connection.cpp connection.h githyanki.h 

# limpeza
clean:
	-rm -f $(objs) *~
purge: clean
	-rm -f rgps

