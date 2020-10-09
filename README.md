# N-puzzle
The N-puzzle problem consists in sorting a board with N tiles initially in a random position. The board is square and has one empty space that allows the tiles to move. This problem was proven to be NP-complete. The solution implements:
* **Best-First**: a greedy algorithm that chooses the best solution at the moment without considering the cost to get to it.
* **A***: considers both the best solution at the moment and the total cost to reach it.

Th heuristics used to compute the cost are:
* Manhattan distance
* Hamming distance

# Compile
To compile the C++ version, use
```
mingw32-make -f makecpp.mk all
```
Or the C version:
```
mingw32-make -f makec.mk all
```
Note 1: this runs on Windows with mingw-32 installed.
Note 2: the C version is **not working**.

# Todo
* Create a Makefile for linux
* Use arguments instead of inside menu
* Correct the C version