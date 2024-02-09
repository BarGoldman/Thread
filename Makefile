.PHONY: all
all: generator primeCounter run

run: performance.o
	gcc -o performance performance.o -lm -lpthread

generator:  generator.c
	gcc -o randomGenerator generator.c

primeCounter:	primeCounter.c
	gcc -o primeCounter primeCounter.c

performance.o: performance.c
	gcc -c performance.c -o performance.o

.PHONY: clean
clean:
	-rm randomGenerator primeCounter -rf *.o performance 2>/dev/null