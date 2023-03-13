#include <stdio.h>
#include <stdlib.h>
#include "AdptArray.h"

#define init 1

struct AdptArray_ {
    int size;
    int cap;
    PElement *data;
    COPY_FUNC copyFunc;
    DEL_FUNC delFunc;
    PRINT_FUNC printFunc;
};

PAdptArray CreateAdptArray(COPY_FUNC copyFunc, DEL_FUNC delFunc, PRINT_FUNC printFunc) {
    PAdptArray adptArray = (PAdptArray) malloc(sizeof(struct AdptArray_));
    if (adptArray == NULL) {
        return NULL;
    }
    adptArray->data = (PElement) malloc(init * sizeof(PElement));
    if (adptArray->data == NULL) {
        free(adptArray);
        return NULL;
    }
    adptArray->size = 0;
    adptArray->cap = init;
    adptArray->data[0] = NULL;
    adptArray->copyFunc = copyFunc;
    adptArray->delFunc = delFunc;
    adptArray->printFunc = printFunc;

    return adptArray;
}

void DeleteAdptArray(PAdptArray adptArray) {
    if (adptArray == NULL)
        return;
    for (int i = 0; i < adptArray->size; ++i) {
        if (adptArray->data[i] == NULL) {
            continue;
        }
        adptArray->delFunc(adptArray->data[i]);
    }
    free(adptArray->data);
    free(adptArray);
}

Result SetAdptArrayAt(PAdptArray adptArray, int i, PElement element) {
    if (i < 0) {
        return FAIL;
    }
    if (i > adptArray->cap) {
        PElement* newData = (PElement*) realloc(adptArray->data, (i+1) * (sizeof (PElement)));
        if (newData == NULL) {
            return FAIL;
        }
        for (int j = adptArray->cap; j < i+1; ++j) {
            newData[j] = NULL;
        }
        adptArray->data = newData;
        adptArray->cap = (i+1);
    }
    if (adptArray->data[i] != NULL) {
        adptArray->delFunc(adptArray->data[i]);
    }
    adptArray->data[i] = adptArray->copyFunc(element);
    adptArray->size = adptArray->cap;
    return SUCCESS;
}

PElement GetAdptArrayAt(PAdptArray adptArray, int i) {
    if (i < 0 || i > adptArray->cap || adptArray->data[i] == NULL) {
        return NULL;
    }
    return adptArray->copyFunc(adptArray->data[i]);
}

int GetAdptArraySize(PAdptArray adptArray) {
    if (adptArray == NULL) {
        return -1;
    }
    return adptArray->size;
}

void PrintDB(PAdptArray adptArray) {
    for (int i = 0; i < adptArray->cap; ++i) {
        if (adptArray->data[i] == NULL) {
            continue;
        }
        adptArray->printFunc(adptArray->data[i]);
    }
}
