#include <stdio.h>
#include <stdlib.h>
#include "AdptArray.h"

#define init 1

struct AdptArray_ {
    int size; // the size of the arry
    PElement *data; //data
    //the build funcs:
    COPY_FUNC copyFunc;
    DEL_FUNC delFunc;
    PRINT_FUNC printFunc;
};

PAdptArray CreateAdptArray(COPY_FUNC copyFunc, DEL_FUNC delFunc, PRINT_FUNC printFunc) {
    PAdptArray adptArray = (PAdptArray) malloc(sizeof(struct AdptArray_)); //create
    if (adptArray == NULL) {
        return NULL;
    }
    adptArray->data = (PElement) malloc(init * sizeof(PElement)); //create data
    if (adptArray->data == NULL) {
        free(adptArray);
        return NULL;
    }
//    adptArray->size = 0; //start from 0
    adptArray->size = init; //no need
    adptArray->data[0] = NULL;
    adptArray->copyFunc = copyFunc;
    adptArray->delFunc = delFunc;
    adptArray->printFunc = printFunc;

    return adptArray;
}

void DeleteAdptArray(PAdptArray adptArray) {
    if (adptArray == NULL)
        return;
    //delete data
    for (int i = 0; i < adptArray->size; ++i) {
        if (adptArray->data[i] == NULL) {
            continue;
        }
        adptArray->delFunc(adptArray->data[i]);
    }
    free(adptArray->data); //free data
    free(adptArray); //free array
}

Result SetAdptArrayAt(PAdptArray adptArray, int i, PElement element) {
    if (i < 0) {
        return FAIL;
    }
    //realloc the array (increase it)
    if (i > adptArray->size) {
        PElement* newData = (PElement*) realloc(adptArray->data, (i+1) * (sizeof (PElement)));
        if (newData == NULL) {
            return FAIL;
        } //make all new to NULL
        for (int j = adptArray->size; j < i + 1; ++j) {
            newData[j] = NULL;
        }
        adptArray->data = newData; //update it
        adptArray->size = (i + 1); //update the size
    }
    if (adptArray->data[i] != NULL) { //if not empty - delete it
        adptArray->delFunc(adptArray->data[i]);
    }
    adptArray->data[i] = adptArray->copyFunc(element);
//    adptArray->size = adptArray->size;
    return SUCCESS;
}

PElement GetAdptArrayAt(PAdptArray adptArray, int i) {
    if (i < 0 || i > adptArray->size || adptArray->data[i] == NULL) {
        return NULL;
    }
    return adptArray->copyFunc(adptArray->data[i]); //return the data
}

int GetAdptArraySize(PAdptArray adptArray) {
    if (adptArray == NULL) {
        return -1;
    }
    return adptArray->size; //return the size
}

void PrintDB(PAdptArray adptArray) {
    for (int i = 0; i < adptArray->size; ++i) {
        if (adptArray->data[i] == NULL) {
            continue;
        }
        adptArray->printFunc(adptArray->data[i]); //print
    }
}
