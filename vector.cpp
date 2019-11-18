#include "vector.h"

int CharToInteger(char a) {
    if (isdigit(a)) {
        return (a - '0');
    } else if (tolower(a)) {
        return (a - 'W');
    } else {
        return -1;
    }
}

bool operator<(TCharVector a, TCharVector b) {
    unsigned long a_decimal = 0;
    unsigned long b_decimal = 0;
    int a_k = 1;
    int b_k = 1;
    int i = 0;
    while (i < a.size) {
        a_decimal += CharToInteger(a.body[i]);
        a_k *= 10;
        i++;
    }
    i = 0;
    while (i < b.size) {
        b_decimal += CharToInteger(b.body[i]);
        b_k *= 10;
        i++;
    }
    return a_decimal < b_decimal;
}

bool operator>(TCharVector a, TCharVector b) {
    unsigned long a_decimal = 0;
    unsigned long b_decimal = 0;
    int a_k = 1;
    int b_k = 1;
    int i = 0;
    while (i < a.size) {
        a_decimal += CharToInteger(a.body[i]);
        a_k *= 10;
        i++;
    }
    i = 0;
    while (i < b.size) {
        b_decimal += CharToInteger(b.body[i]);
        b_k *= 10;
        i++;
    }
    return a_decimal > b_decimal;
}

bool operator==(TCharVector a, TCharVector b) {
    unsigned long a_decimal = 0;
    unsigned long b_decimal = 0;
    int a_k = 1;
    int b_k = 1;
    int i = 0;
    while (i < a.size) {
        a_decimal += CharToInteger(a.body[i]);
        a_k *= 10;
        i++;
    }
    i = 0;
    while (i < b.size) {
        b_decimal += CharToInteger(b.body[i]);
        b_k *= 10;
        i++;
    }
    return a_decimal == b_decimal;
}
TCharVector* TCreate() {
    TCharVector* v = new TCharVector;
    v->body = nullptr;
    v->size = 0;
    v->cap = 1;
    return v;
}
void TDelete(TCharVector* v) {
    delete(v->body);
    delete(v);
}
void TSet(TCharVector* v, int i, char val) {
    v->body[i] = val;
}
char TGet(TCharVector* v, int i) {
    return v->body[i];
}
int TGetSize(TCharVector* v) {
    return v->size;
}
int TGetCap(TCharVector* v) {
    return v->cap;
}
void TSetSize(TCharVector* v, int new_size) {
    v->size = new_size;
}
void TSetCap(TCharVector* v, int new_size) {
    char* reBody = (char*)realloc(v->body, new_size * sizeof(char));
    v->body = reBody;
}
