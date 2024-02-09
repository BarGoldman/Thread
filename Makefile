.PHONY: all
all: generator primeCounter run

run: main.o
	gcc -o main main.o -lm -lpthread

generator:  generator.c
	gcc -o randomGenerator generator.c

primeCounter:	primeCounter.c
	gcc -o primeCounter primeCounter.c

main.o: main.c
	gcc -c main.c -o main.o

.PHONY: clean
clean:
	-rm randomGenerator primeCounter 2>/dev/null