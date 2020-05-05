# Makefile
# Řešení IOS, 23.04.2020
# Autor: Milan Koval, FIT
# Přeloženo: gcc 7.5.0

CFLAGS = -std=gnu99 -pedantic -Wall -Wextra -g -O0
CC = gcc

default: run
	cat proj2.out

dbg: proj2.c
	$(CC) $(CFLAGS) -pthread -DDEBUG  proj2.c -o proj2 -lrt
	./proj2 5 2 7 1 1

run: proj2
	./proj2 5 2 7 1 1

proj2: proj2.c
	$(CC) $(CFLAGS) -pthread  proj2.c -o proj2 -lrt
