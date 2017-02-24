#include <math.h>
#include <time.h>

#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <queue>
#include <iomanip>
#include <string>
#include <utility>

#include "puzzle.h"

using namespace std;

Puzzle::Puzzle (int width) {
    // Insert the tiles on the vector
    for (int i = 1; i < pow(width, 2); i++)
        this->config.push_back(i);
    this->config.push_back(0);

    // Sets initial configurations of the puzzle
    this->width     = width;
    this->cost      = 0;
    this->heuristic = this->calcHeuristics();
    this->action    = -1;
    this->parent    = NULL;
    this->blank     = this->getXY(pow(width, 2) - 1);
}

void Puzzle::randomize() {
    // Randomize the tiles in the vector
    srand(unsigned(time(0)));
    random_shuffle(this->config.begin(), this->config.end());

    // Finds new position of the blank space
    this->blank = this->getXY(find(this->config.begin(), this->config.end(), 0) - this->config.begin());

    // While the randomized vector is unsolvable, randomize it again!
    if (this->isUnsolvable())
        this->randomize();
}

pair<int, int> Puzzle::getXY (int pos) {
    // The vector that stores the tiles, is linear. This function returns
    // the position of a tile considering the vector as a matrix width*width
    if (++pos % this->width == 0)
        return make_pair(pos/this->width - 1, this->width - 1);
    return make_pair((pos - pos % this->width)/this->width, pos % this->width - 1);
}

int Puzzle::isUnsolvable () {
    int inv = 0, s = pow(this->width, 2), n = this->width;
    vector<int>::iterator i, j;

    // Count number of inversions on the puzzle (see the report)
    for (i = this->config.begin(); i < this->config.end() - 1; ++i)
        for (j = i + 1; j < this->config.end(); ++j)
            if (*j < *i && *j != 0)
                inv++;

    return !(((n % 2 != 0) && (inv % 2 == 0)) || ((n % 2 == 0) && (this->blank.first % 2 != inv % 2)));
}

int Puzzle::canAct (int action) {
    // Conditions to make a move
    int arr[4] = {this->blank.first != 0, this->blank.first != this->width - 1, this->blank.second != 0, this->blank.second != this->width - 1};

    return arr[max(0, min(3, action))];
}

void Puzzle::doAction (int action) {
    action = max(0, min(3, action));
    // Get current position of the blank space
    int pos = this->blank.first * this->width + this->blank.second;

    // Define the possible positions for the blank space
    int coord[4] = {pos - this->width, pos + this->width, pos - 1, pos + 1};
    // Choses one of the positions above
    int cho = coord[action];

    // updates all informations of the puzzle
    this->cost++;

    this->config[pos] = this->config[cho];
    this->config[cho] = 0;
    this->blank       = this->getXY(cho);
    this->heuristic   = this->calcHeuristics();
    this->action      = action;
}

int Puzzle::calcHeuristics () {
    int mhd = 0, msp = 0, j, i;
    pair<int, int> pos, goal;

    for (i = 0; i < pow(this->width, 2); i++) {
        // This tile is in (i) position;
        pos  = this->getXY(i);
        // Calculates the goal position for it;
        goal = this->getXY(this->config[i] - 1);
        // If the tile is a blank space, the goal position for it, is the last on the puzzle
        if (this->config[i] == 0) goal = make_pair(this->width - 1, this->width - 1);

        // Manhattan Distance
        mhd += abs(pos.first - goal.first) + abs(pos.second - goal.second);
        // Misplaced Tiles (Hamming Distance)
        if (this->config[i] != (i + 1)) msp += 1;
    }
    if (this->config[i - 1] == 0) msp -= 1;

    return mhd;
}

void Puzzle::makeClone (Puzzle *p) {
    p->cost      = this->cost;
    p->heuristic = this->heuristic;
    p->config    = this->config;
    p->blank     = this->blank;
    p->parent    = this;
}

void Puzzle::printPuzzle () {
    for (int i = 0; i < pow(this->width, 2); i++) {
        cout << setw(2) << this->config[i] << ' ';
        if ((i + 1) % this->width == 0) cout << endl;
    }
}

int Puzzle::getHeuristic () {
    return this->heuristic+this->cost;
}






struct comparator {
    bool operator() (Puzzle* a, Puzzle* b) {
        return a->getHeuristic() > b->getHeuristic();
    }
};

Puzzle *solver (Puzzle *initial, Puzzle *goal, int *expanded) {
    // The problem border is a min priority queue
    priority_queue<Puzzle*, vector<Puzzle*>, comparator> border;
    // Stores the visited nodes
    vector<vector<int> > expand;
    Puzzle *current;
    int i;

    border.push(initial);

    while (border.size() != 0) {
        current = border.top();
        border.pop();

        if (current->config == goal->config)
            return current;

        expand.push_back(current->config);
        (*expanded)++;

        for (i = 0; i < 4; i++) {
            if (current->canAct(i)) {
                Puzzle *clone = new Puzzle(current->width);
                current->makeClone(clone);
                clone->doAction(i);

                if (current->config == goal->config)
                    return clone;
                else if (find(expand.begin(), expand.end(), clone->config) == expand.end())
                    border.push(clone);
                else
                    free(clone);
            }
        }
    }

    return NULL;
}



void showMoves(Puzzle *p) {
    char ans;
    int k = 0;
    vector<int> moves;
    string movements[4] = {"up", "down", "left", "right"};

    if (p->cost != 0) {
        cout << "Do you want to show the moves? (y/n) ";
        cin >> ans;
        cout << endl;
        if (ans == 'y' || ans == 'Y') {
            while (p) {
                if (p->action > -1)
                    moves.push_back(p->action);
                p = p->parent;
            }
            reverse(moves.begin(), moves.end());
            for (vector<int>::iterator i = moves.begin(); i != moves.end(); ++i, ++k) {
                cout << setw(9) << movements[(*i)];
                if ((k + 1) % 6 == 0) cout << endl;
            }
            cout << endl << endl;
        }
    }
}


int main() {
    int expanded = 0;
    Puzzle *initial = new Puzzle(3);
    Puzzle *goal    = new Puzzle(3);
    Puzzle *answer;

    initial->randomize();

    int v[9] = {5, 6, 4, 3, 7, 1, 0, 8, 2};
    for (int i = 0; i < 9; i++)
        initial->config[i] = v[i];
    initial->blank = initial->getXY(6);
    initial->heuristic = initial->calcHeuristics();

    cout << "Initial State:" << endl;
    initial->printPuzzle();
    cout << endl;

    cout << "Final State:" << endl;
    clock_t t = clock();
    if (answer = solver(initial, goal, &expanded)) {
        answer->printPuzzle();
        t = clock() - t;

        cout << endl << "Execution Time  (s)     " << "Moved Tiles     " << "Nodes Expanded" << endl;
        cout << setw(19) << ((float)t)/CLOCKS_PER_SEC << "     " << setw(11) << answer->cost << "     ";
        cout << setw(14) << expanded << endl << endl;

        showMoves(answer);
    } else
        cout << " The answer was not found!" << endl;

    return 0;
}
