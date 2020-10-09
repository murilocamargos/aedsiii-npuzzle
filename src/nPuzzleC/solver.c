#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "list.h"
#include "heap.h"
#include "solver.h"

void *solverHeu(List *border, List *expand, void *obj, int n, Cmp *cmp, Cmp *heu, Cloner *clo, CanAct *cAct, Action *act) {
    void *state, *clone;
    int i;

    while (border->size != 0) {

        state = heapTop(border, heu);
        lstAdd(expand, state);

        for (i = 0; i < n; i++) {
            if (cAct(state, i)) {
                clone = clo(state);
                act(clone, i);

                if (cmp(clone, obj))
                    return clone;
                else if (!lstFind(expand, clone, cmp))
                    heapAdd(border, clone, heu);
                else
                    free(clone);
            }
        }
    }

    return NULL;
}

void *solver(List *border, List *expand, void *obj, int n, Cmp *cmp, Cloner *clo, CanAct *cAct, Action *act, Extract *ext) {
    void *state, *clone;
    int i;

    while (border->size != 0) {
        state = ext(border);
        lstAdd(expand, state);

        for (i = 0; i < n; i++) {
            if (cAct(state, i)) {
                clone = clo(state);
                act(clone, i);

                if (cmp(clone, obj))
                    return clone;
                else if (!lstFind(expand, clone, cmp))
                    lstAdd(border, clone);
            }
        }
    }

    return NULL;
}
