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
	$(CC) $(CPPFLAGS) $(LDLIBS) $(objs) -o $(TARGET)

# compilacao
main.o: githyanki.h
socket.o: socket.h
githyanki.o: githyanki.h
checksum.o: errors.h
connection.o: connection.h githyanki.h

# limpeza
clean:
	-rm -f $(objs) *~
purge: clean
	-rm -f $(TARGET)

