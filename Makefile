# Makefile
# feito por Gabriel Pontarolo
CC = g++
TARGET = rawSocket

# flags
CPPFLAGS = -Wall -g --std=c++17
LDLIBS = -lm -lgmp

# diretorios fonte
VPATH = src:src/sockets:src/error_handling

objs = main.o githyanki.o connection.o common.o socket.o crc.o

.PHONY: all clean purge

all: $(TARGET)

# ligacao
$(TARGET): $(objs)
	$(CC) $(CPPFLAGS) $(LDLIBS) $(objs) -o $(TARGET)

# compilacao
main.o: main.cpp connection.h githyanki.h common.h
socket.o: socket.c socket.h
common.o: common.cpp common.h
githyanki.o: githyanki.cpp githyanki.h connection.h common.h errors.h
connection.o: connection.cpp connection.h githyanki.h 
crc.o: crc.cpp errors.h


# limpeza
clean:
	-rm -f $(objs) *~
purge: clean
	-rm -f $(TARGET)

