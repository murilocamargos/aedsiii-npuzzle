#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "lib/list.h"
#include "lib/solver.h"
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

/* The puzzle is represented by a n-by-n matrix. The blank space
   is represented by the integer zero. And the required state is
   when the blank space is in the last position of the matrix. */
typedef struct _puz {
    int **config, n, bx, by, cost;
    struct _puz *father;
    char act[15];
} Puzzle;

/* =========================== PROTOTYPES =========================== */

int puzIni(Puzzle *p);
void puzObj(Puzzle *p);

void *puzClone(void *itm);

int puzCanMove(void *itm, int act);
void puzMove(void *itm, int act);

int puzEqual(void *itm1, void *itm2);
int puzMisplaced(Puzzle *p);
int puzManhattan(Puzzle *p);
int puzHeuristics(void *itm1, void *itm2);
int puzIsSolvable(Puzzle *p);

void puzPrint(Puzzle *p);

/* =========================== STATES CREATION =========================== */

/* Initializes a Puzzle Board with a random configuration */
int puzIni(Puzzle *p) {
    int i, j;
    List tiles;

    lstIni(&tiles);
    for (i = 0; i < p->n*p->n; i++)
        lstAdd(&tiles, i);

    p->father = NULL;
    strcpy(p->act, "");
    p->cost = 0;

    p->config = (int **)calloc(p->n, sizeof(int));
    if (!p->config) return 0;

    for (i = 0; i < p->n; i++) {
        p->config[i] = (int *)calloc(p->n, sizeof(int));
        if (!p->config[i]) return 0;
    }

    for (i = 0; i < p->n; i++) {
        for (j = 0; j < p->n; j++) {
            p->config[i][j] = lstRemoveRand(&tiles);
            if (p->config[i][j] == 0) {
                p->bx = i;
                p->by = j;
            }
        }
    }
    return 1;
}

/* Makes a clone of a Puzzle */
void *puzClone(void *itm){
    Puzzle *p = itm, *puz;
    int i, j;

    puz = (Puzzle*) malloc (sizeof(Puzzle));
    if (!puz) return NULL;

    puz->n = p->n;

    puzIni(puz);

    puz->father = p;
    puz->bx = p->bx;
    puz->by = p->by;
    puz->cost = p->cost;

    for (i = 0; i < p->n; i++)
        for (j = 0; j < p->n; j++)
            puz->config[i][j] = p->config[i][j];

    return puz;
}

/* Transform any given puzzle in the goal puzzle */
void puzObj(Puzzle *p) {
    int i, j, k = 1;
    for (i = 0; i < p->n; i++)
        for (j = 0; j < p->n; j++)
            p->config[i][j] = k++;
    p->config[p->n-1][p->n-1] = 0;
    p->bx = p->by = p->n-1;
}

/* =========================== STATES ACTIONS =========================== */

/* Verifies if a puzzle can performs a given action;
   The actions are:
   0 -> move the blank space to the top;
   1 -> move the blank space to the bottom;
   2 -> move the blank space to the left;
   3 -> move the blank space to the right; */
int puzCanMove(void *itm, int act){
    Puzzle *p = itm;
    if (act == 0 && p->bx != 0) return 1;
    if (act == 1 && p->bx < p->n-1) return 1;
    if (act == 2 && p->by != 0) return 1;
    if (act == 3 && p->by < p->n-1) return 1;
    return 0;
}

/* Performs an action on the given Puzzle. */
void puzMove(void *itm, int act) {
    Puzzle *p = itm;
    if (puzCanMove(p, act)) {
        if (act == 0) {
            p->config[p->bx][p->by] = p->config[p->bx-1][p->by];
            p->config[--p->bx][p->by] = 0;
            strcpy(p->act, "Para Cima");
        } else if (act == 1) {
            p->config[p->bx][p->by] = p->config[p->bx+1][p->by];
            p->config[++p->bx][p->by] = 0;
            strcpy(p->act, "Para Baixo");
        } else if (act == 2) {
            p->config[p->bx][p->by] = p->config[p->bx][p->by-1];
            p->config[p->bx][--p->by] = 0;
            strcpy(p->act, "Para Esquerda");
        } else {
            p->config[p->bx][p->by] = p->config[p->bx][p->by+1];
            p->config[p->bx][++p->by] = 0;
            strcpy(p->act, "Para Direita");
        }
        p->cost++;
    }
}


/* ======================== STATES VERIFICATIONS ======================== */

/* Verifies if two given states (Puzzle Board) are equal */
int puzEqual(void *itm1, void *itm2) {
    int i, j;
    Puzzle *p1 = itm1, *p2 = itm2;
    for (i = 0; i < p1->n; i++)
        for (j = 0; j < p1->n; j++)
            if ((int)p1->config[i][j] != (int)p2->config[i][j]) return 0;
    return 1;
}

/* Verifies how many tiles ares misplaced in the given Puzzle */
int puzMisplaced(Puzzle *p){
    int msp = 0, i, j, k = 1;
    for (i = 0; i < p->n; i++) {
        for (j = 0; j < p->n; j++) {
            if (i == p->n-1 && j == p->n-1) {
                if (p->config[i][j] != 0) msp++;
            } else if (p->config[i][j] != k++) msp++;
        }
    }
    return msp;
}

/* Verifies the Manhattan distance between the current position of a tile
   and the goal position of the same tile */
int puzManhattan(Puzzle *p) {
    int i, j, x, y, d = 0;
    for (i = 0; i < p->n; i++) {
        for (j = 0; j < p->n; j++) {
            x = p->config[i][j]/p->n;
            if (p->config[i][j] % p->n == 0 && p->config[i][j] != 0) x--;
            y = p->config[i][j];
            while (y > p->n)
                y -= p->n;
            y--;
            if (p->config[i][j] == 0) x = y = p->n-1;
            d += (abs(x-i) + abs(y-j));
        }
    }
    return d;
}

/* Verifies which state (of the two given states) approximate
   the most of the goal state */
int puzHeuristics(void *itm1, void *itm2){
    Puzzle *i1 = itm1, *i2 = itm2;
    int h11, h12, h21, h22;
    h11 = puzManhattan(i1);
    h12 = puzManhattan(i2);
    h21 = puzMisplaced(i1);
    h22 = puzMisplaced(i2);
    if (h21+i1->cost < h22+i2->cost) return 1;
    //if (h21 + itm1->cost < h22 + itm2->cost) return 1;
    return 0;
}


/* Prints a given state */
void puzPrint(Puzzle *p) {
    int i, j;
    if (!p)
        printf("NULO!");
    else {
        for (i = 0; i < p->n; i++) {
            for (j = 0; j < p->n; j++) printf("+--");
            printf("+\n|");
            for (j = 0; j < p->n; j++)
                if (p->config[i][j] != 0) printf("%2d|", p->config[i][j]);
                else printf("  |");
            printf("\n");
        }
        for (j = 0; j < p->n; j++) printf("+--");
        printf("+\n\n");
    }
}

/* Verifies if a given puzzle is solvable */
int puzIsSolvable(Puzzle *p) {
    List tiles;
    Node *a, *nxt;
    int i, j;

    lstIni(&tiles);

    for (i = 0; i < p->n; i++)
        for (j = 0; j < p->n; j++)
            lstAdd(&tiles, p->config[i][j]);

    a = tiles.first;
    i = j = 0;
    while (a) {
        nxt = lstGet(&tiles, ++i);
        while (nxt) {
            if (nxt->itm < a->itm && nxt->itm != 0) j++;
            nxt = nxt->next;
        }
        a = a->next;
    }

    return (((p->n % 2 != 0) && (j % 2 == 0)) || ((p->n % 2 == 0) && (p->bx % 2 != j % 2)));
}


int main() {
    int i;
    Puzzle puz, obj, *result;
    List border, expand;
    srand(time(NULL));

    lstIni(&border);
    lstIni(&expand);

    clock_t tempo;

    puz.n = obj.n = 3;
    puzIni(&puz);
    puzIni(&obj);
    puzObj(&obj);

    puzPrint(&puz);
    if (puzIsSolvable(&puz)) {

        lstAdd(&border, (void*)&puz);

        tempo = clock();
        result = solverHeu(&border, &expand, &obj, 4, puzEqual, puzHeuristics, puzClone, puzCanMove, puzMove);
        tempo = clock() - tempo;

        puzPrint(result);

        i = -1;
        while (result) {
            //printf("%s\n", result->act);
            result = result->father;
            i++;
        }

        printf("\n\nThe algorithm moved the tiles %d time(s)!\nTook %f seconds to solve the problem, and expanded %d states.\n\n\n", i, ((float)tempo)/CLOCKS_PER_SEC, expand.size);

    } else
        printf("The random puzzle is not solvable. Please, execute the algorithm again!\n");

    return 0;
}
