#ifndef __VALUES_H__
#define __VALUES_H__

#include "common.h"

/*
 * This represents the space of memory to store constants and data that are not instructions. It is represented as the right pile
 * inside the memBlock.h
*/

typedef double gdelData;

typedef enum {
    DT_BOOL,
    DT_NUMBER, 
    DT_NULL
} gdelDataType;

typedef struct {
    size_t maxSize;
    size_t currentDataValue;
    gdelData* pool;
} gdelDataPool;

void initGdelDataPool(gdelDataPool* _dataPool);
void writeToDataPool(gdelDataPool* _dataPool, gdelData _data);
void freeDataPool(gdelDataPool* _dataPool);

#endif // __VALUES_H__
