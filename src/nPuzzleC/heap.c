#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "heap.h"

void heapBuild(List *a, Cmp *cmp) {
    int i;
    for(i = a->size/2; i >= 0; i--)
        heapHeapify(a, i, cmp);
}

void heapHeapify(List *a, int i, Cmp *cmp) {
    int left, right, analy;
    left = 2*i + 1;
    right = 2*i + 2;
    analy = i;
    if (left < a->size && cmp(lstGetItm(a, left), lstGetItm(a, analy)))
        analy = left;
    if (right < a->size && cmp(lstGetItm(a, right), lstGetItm(a, analy)))
        analy = right;
    if (analy != i) {
        lstNodeSwap(a, i, analy);
        heapHeapify(a, analy, cmp);
    }
}

void *heapTop(List *a, Cmp *cmp) {
    Node *aux = lstRemoveAt(a, 0);
    if (!aux) return NULL;
    if (a->size > 0) heapBuild(a, cmp);
    else if (a->size == 0) lstIni(a);
    return aux->itm;
}

void heapAdd(List *a, void *itm, Cmp *cmp) {
    lstAdd(a, itm);
    heapBuild(a, cmp);
}

