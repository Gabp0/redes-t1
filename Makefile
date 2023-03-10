# Makefile
# feito por Gabriel Pontarolo
CC = g++
TARGET = chat

# flags
CPPFLAGS = -Wall -g --std=c++17
LDLIBS = -lm -lgmp -lncurses -ltinfo -lpthread

# diretorios fonte
VPATH = src:src/sockets:src/error_handling:src/utils:src/application:src/chat

objs = main.o githyanki.o connection.o common.o socket.o crc.o application.o chat.o
#objs = main.old.o githyanki.o connection.o common.o socket.o crc.o application.o chat.o

.PHONY: all clean purge

all: $(TARGET)

# ligacao
$(TARGET): $(objs)
	$(CC) $(objs) $(CPPFLAGS) $(LDLIBS) -o $(TARGET)

# compilacao
main.o: connection.h githyanki.h common.h
# main.old.o: connection.h githyanki.h common.h
socket.o: socket.h
common.o: common.h
githyanki.o: githyanki.h connection.h common.h errors.h
connection.o: connection.h githyanki.h 
application.o: application.h
chat.o: chat.h application.h
crc.o: errors.h


# limpeza
clean:
	-rm -f $(objs) *~
purge: clean
	-rm -f $(TARGET)

