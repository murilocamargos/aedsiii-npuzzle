#ifndef _LIST_H
#define _LIST_H

typedef struct _node {
    void *itm;
    struct _node *next, *prev;
} Node;

typedef struct {
    int size;
    Node *first, *last;
} List;

typedef void (Printer)(void *itm);
typedef int  (Cmp)(void *itm1, void *itm2);

/** \brief Initializes the list.
 *
 * \param	List *: A List pointer.
 * \return	void
 *
 * Receives a List pointer of a preallocated list and sets the size equals zero and the first equals the last element equals NULL.
 */
void lstIni(List *lst);

List *lstNew();

Node *lstNewNode(void *itm);
int lstAdd(List *lst, void *itm);

Node *lstGet(List *lst, int p);
void *lstGetItm(List *lst, int p);

int lstGetRandPos(List *lst);
Node *lstGetRand(List *lst);
void *lstGetRandItm(List *lst);

Node *lstRemoveAt(List *lst, int p);
void *lstRemoveFifo(List *lst);
void *lstRemoveLifo(List *lst);
void *lstRemoveRand(List *lst);
void *lstRemoveItm(List *lst, void *itm, Cmp *cmp);

void lstPrintList(List *lst, Printer *prt);
void lstPrintNode(Node *nd, Printer *prt);

void lstNodeSwap(List *lst, int p1, int p2);

int lstFind(List *lst, void *itm, Cmp *cmp);
void *lstFindItm(List *lst, void *itm, Cmp *cmp);

#endif
