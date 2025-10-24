#ifndef _ZBIOR_ARY_H_
#define _ZBIOR_ARY_H_

#include <stdbool.h>

typedef struct {
    int a, b, r;
} Segment;

typedef struct zbior_ary {
    Segment* data;
    int size;
    int cap;
} zbior_ary;

/* API z zadania – bez zmian */
zbior_ary ciag_arytmetyczny(int a, int q, int b);
zbior_ary singleton(int a);
zbior_ary suma(zbior_ary A, zbior_ary B);
zbior_ary iloczyn(zbior_ary A, zbior_ary B);
zbior_ary roznica(zbior_ary A, zbior_ary B);
bool nalezy(zbior_ary A, int b);
unsigned moc(zbior_ary A);
unsigned ary(zbior_ary A);

#endif
