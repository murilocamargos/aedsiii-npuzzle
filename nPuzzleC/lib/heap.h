#ifndef _HEAP_H
#define _HEAP_H

void heapBuild(List *a, Cmp *cmp);
void heapHeapify(List *a, int i, Cmp *cmp);

void heapAdd(List *a, void *itm, Cmp *cmp);

// Remove and return the first element of the heap
void *heapTop(List *a, Cmp *cmp);

#endif
