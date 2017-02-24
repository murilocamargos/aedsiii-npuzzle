#ifndef _PUZZLE_H
#define _PUZZLE_H

using namespace std;

class Puzzle {
    public:
        /** Attributes **/
        int heuristic, width, cost, action;
        pair<int, int> blank;
        Puzzle *parent;
        vector<int> config;

        /** Methods **/
        /* Initialize a puzzle with width^2 elements */
        Puzzle(int width);
        /* Randomize the elements of the puzzle */
        void randomize();
        /* Verifies if the blank tile can move to some direction */
        int canAct(int action);
        /* Moves the blank tile to somewhere */
        void doAction(int action);
        /* Copy this informations to the *p Puzzle */
        void makeClone(Puzzle *p);
        /* Prints the puzzle */
        void printPuzzle();
        /* Returns the value of the heuristic previously calculated */
        int getHeuristic();
        /* Gets the positions of the line and column of an element */
        pair<int, int> getXY(int pos);
        /* Verifies if the puzzle is solvable */
        int isUnsolvable();
        /* Calculates the heuristics value */
        int calcHeuristics();
};

#endif
