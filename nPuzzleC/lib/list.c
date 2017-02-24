#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "list.h"

void lstIni(List *lst) {
    lst->size = 0;
    lst->first = lst->last = NULL;
}

List *lstNew() {
    List *nl;

    nl = (List *) malloc (sizeof(List));
    if (!nl) return NULL;

    lstIni(nl);
    return nl;
}

Node *lstGet(List *lst, int p) {
    int i = 0;
    Node *aux;

    aux = lst->first;
    while (aux) {
        if (p == i++) return aux;
        aux = aux->next;
    }

    return NULL;
}

Node *lstNewNode(void *itm) {
    Node *nd;

    nd = (Node *) malloc (sizeof(Node));
    if (!nd) return NULL;

    nd->itm = itm;
    nd->next = nd->prev = NULL;

    return nd;
}

Node *lstRemoveAt(List *lst, int p) {
    if (lst->size == 0 || p >= lst->size || p < 0) return NULL;

    Node *nd = lstGet(lst, p), *aux;
    if (!nd) return NULL;

    aux = nd;
    if (lst->size == 1) {
        lstIni(lst);
        lst->size++;
    } else if (!nd->prev) {
        nd->next->prev = NULL;
        lst->first = nd->next;
    } else if (!nd->next) {
        nd->prev->next = NULL;
        lst->last = nd->prev;
    } else {
        nd->prev->next = aux->next;
        nd->next->prev = aux->prev;
    }

    lst->size--;

    return nd;
}

void *lstRemoveFifo(List *lst) {
    Node *aux = lstRemoveAt(lst, 0);
    if (aux) return aux->itm;
    return NULL;
}
void *lstRemoveLifo(List *lst) {
    Node *aux = lstRemoveAt(lst, lst->size-1);
    if (aux) return aux->itm;
    return NULL;
}

int lstGetRandPos(List *lst) {
    srand(time(NULL));
    return (rand() % lst->size);
}
Node *lstGetRand(List *lst) {
    return lstGet(lst, lstGetRandPos(lst));
}
void *lstGetRandItm(List *lst) {
    Node *aux;
    aux = lstGetRand(lst);
    return aux->itm;
}
void *lstRemoveRand(List *lst) {
    Node *aux;
    aux = lstRemoveAt(lst, lstGetRandPos(lst));
    return aux->itm;
}

void *lstGetItm(List *lst, int p) {
    Node *aux = lstGet(lst, p);
    if (aux) return aux->itm;
    return NULL;
}

int lstAdd(List *lst, void *itm) {
    Node *nd;

    nd = lstNewNode(itm);
    if (!nd) return 0;

    nd->next = NULL;

    if (lst->size == 0) {
        nd->prev = nd->next;
        lst->first = lst->last = nd;
    } else {
        nd->prev = lst->last;
        lst->last->next = nd;
        lst->last = nd;
    }

    lst->size++;

    return 1;
}

void lstPrintList(List *lst, Printer *prt) {
    Node *aux;

    aux = lst->first;
    while (aux) {
        prt(aux->itm);
        aux = aux->next;
    }
}

void lstPrintNode(Node *nd, Printer *prt) {
    if (!nd) printf("NULL Node!");
    else prt(nd->itm);
}

void lstNodeSwap(List *lst, int p1, int p2) {
    Node *n1, *n2;
    void *aux;

    n1 = lstGet(lst, p1);
    n2 = lstGet(lst, p2);

    if (n1 && n2) {
        aux = n1->itm;
        n1->itm = n2->itm;
        n2->itm = aux;
    }
}

int lstFind(List *lst, void *itm, Cmp *cmp) {
    Node *aux;
    aux = lst->first;
    while (aux) {
        if (cmp(aux->itm, itm)) return 1;
        aux = aux->next;
    }
    return 0;
}

void *lstFindItm(List *lst, void *itm, Cmp *cmp) {
    Node *aux;
    aux = lst->first;
    while (aux) {
        if (cmp(aux->itm, itm)) return aux->itm;
        aux = aux->next;
    }
    return NULL;
}

void *lstRemoveItm(List *lst, void *itm, Cmp *cmp) {
    Node *aux;
    int i = 0;
    aux = lst->first;
    while (aux) {
        if (cmp(aux->itm, itm)) {
            lstRemoveAt(lst, i);
            return aux->itm;
        }
        i++;
        aux = aux->next;
    }
    return NULL;
}
