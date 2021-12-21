CFLAGS= -Wall -Wconversion -Werror

all: serveur client1 client2 client3

serveur: serveur.c header.h serveur.h
client1: client1.c header.h

client2: client2.c header.h

client3: client3.c header.h

.PHONY: all

