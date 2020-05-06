# Makefile
# Řešení IOS, 23.04.2020
# Autor: Milan Koval, FIT
# Přeloženo: gcc 7.5.0

CFLAGS = -std=gnu99 -pedantic -Wall -Wextra -Werror
CC = gcc

default: run
	cat proj2.out

dbg: proj2.c
	$(CC) $(CFLAGS) -pthread -DDEBUG  proj2.c -o proj2 -lrt
	./proj2 5 1000 1000 1000 1000

sortdiff: run
	cat proj2.out | sort -n > /tmp/proj2.s
	diff proj2.out /tmp/proj2.s  

test: run scriptos
	./scriptos < proj2.out



run: proj2
	./proj2 10000 0 0 0 0
	
proj2: proj2.c
	$(CC) $(CFLAGS) -pthread  proj2.c -o proj2 -lrt

scriptos: scriptos.c
	$(CC) -std=gnu99 -pedantic -pthread  scriptos.c -o scriptos -lrt

clean:
	rm scriptos proj2







sortdiff900:
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff
	make sortdiff


test900:
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
	make test
