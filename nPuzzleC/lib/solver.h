#ifndef _SOLVER_H
#define _SOLVER_H

typedef void *(Extract)(List *lst);
typedef void *(Cloner)(void *itm);
typedef int  (CanAct) (void *itm, int t);
typedef void (Action) (void *itm, int t);
typedef void (Add) (List *lst, void *itm, Cmp *cmp);

void *solverHeu(List *border, List *expand, void *obj, int n, Cmp *cmp, Cmp *heu, Cloner *clo, CanAct *cAct, Action *act);

void *solver(List *border, List *expand, void *obj, int n, Cmp *cmp, Cloner *clo, CanAct *cAct, Action *act, Extract *ext);

#endif
