CC=gcc
CFLAGS=-I.

generate: generate.c
	$(CC) -o generate generate.c 

