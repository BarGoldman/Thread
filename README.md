# Ex2 - Thread

<img src="https://github.com/BarGoldman/find-breakdown-EX9/assets/93201414/72a4c4b9-fb7c-48d7-9e0e-dbc3dd94da14.png" width="100" height="100" />

In this task you should train data structures and threading skills.
The background for this task is this:
You have a endless datastream that should be processed fast. Say a security camera data, where an
intrusion should be detected.
In our assinment we will use a random number generator (provided) that simulates the endless stream,
and your task is to count the number of prime numbers in this stream.
The idea is to parallelize the process, so all the cpu core are utilized.
You may improve the isPrime function too, if nessesary.
Note that you allowed to use only 2MB of ram space.
You can implement the solution in any language you want but:
a) beat my best results (on your pc) and explain why this happens:
b) be able to explore the cpu and memory consumption of your solution in details (exceptc c/c++).
your finding should be presented in class (in other words, show us how you can deeply explore the
structure of a language you have used, not just a debuger)
The task in details:
You are given a random number generator called randomGenerator (provided).
You also get a basic implementation of the prime counter app, called primeCounter (provided).
The useage is like this:
randomGenerator <seed> <num_of numbers>.
F.ex randomGenerator 10 10 will give you ten numbers, generated with random seed “10”.
primeCounter don’t need any arguments, but it supposed to get the numbers with std in.
So the usage with the random generator looks like this:
./randomGenerator 10 100 | ./primesCounter
the expected output is : “5 total primes.”
the amount of numbers that likely to be tested is between 1 to 1000 milions of numbers,
so performance is the name of the game.
You can use the “time” command to check the time it takes to calc the primes.
