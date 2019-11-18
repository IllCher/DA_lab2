#ifndef DA_LAB2_VECTOR_H
#define DA_LAB2_VECTOR_H
#include <cstdlib>
#include "ctype.h"
typedef struct {
    char* body;
    int size;
    int cap;
} TCharVector;
int CharToInteger(char a);
TCharVector* TCreate();
void TDelete(TCharVector* v);
void TSet(TCharVector* v, int i, char val);
char TGet(TCharVector* v, int i);
int TGetSize(TCharVector* v);
int TGetCap(TCharVector* v);
void TSetSize(TCharVector* v, int new_size);
void TSetCap(TCharVector* v, int new_size);
#endif //DA_LAB2_VECTOR_H
