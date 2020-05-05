# Makefile
# Řešení IOS, 23.04.2020
# Autor: Milan Koval, FIT
# Přeloženo: gcc 7.5.0

CFLAGS = -std=gnu99 -pedantic -Wall -Wextra -Werror
CC = gcc

proj2: proj2.c
	$(CC) $(CFLAGS) -pthread  proj2.c -o proj2 -lrt

clean:
	rm proj2
