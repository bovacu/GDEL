#ifndef __VALUES_H__
#define __VALUES_H__

#include "common.h"

typedef struct gdelRegister gdelRegister;
typedef struct gdelStringRegister gdelStringRegister;

/*
 * This represents the space of memory to store constants and data that are not instructions. It is represented as the right pile
 * inside the memBlock.h
*/

typedef enum {
    DT_BOOL,
    DT_NUMBER, 
    DT_NULL,
    DT_REGISTER
} gdelDataType;

/*
 * gdelData is the struct that will hold any basic information, this means it can be at the same time an integer, a bool, a double, a char... How do we achieve this? Well, first we need to
 * know what type we are working with, for that we have gdelDataType. Then we need to represent the actual value, and we need that value to be whatever we need, depending on the case.
 *
 * A good way to achieve this is via union. Union allows as for a single variable to be of different types, by using the amount of bytes that the biggest type inside the union has, so if we have 
 * double, int, bool and char, everything will be 8 bytes.
 *
 *                                      double 8 bytes
 *
 *          |───────────────────────────────────────────────────────────────────────|
 *
 *          ╔════════╦════════╦════════╦════════╦════════╦════════╦════════╦════════╗
 *          ║   b0   ║   b1   ║   b2   ║   b3   ║   b4   ║   b5   ║   b6   ║   b7   ║               
 *          ╚════════╩════════╩════════╩════════╩════════╩════════╩════════╩════════╝    
 *
 *          |───────|    
 *
 *          bool 1 byte
 *
 * The other way of doing this could be (in basic cases) with just a double, double would represent int, long, float, double with the type of DT_NUMBER, then DT_BOOL would be 0 and 1, DT_NULL
 * would be 0, and CHARS would hold its ascii value. In any case we don't save any space so doing the union or doing this way is basically the same.
 *
 * The never-go approach is by having a type of each variable, one variable for int, float, bool, double, char...
 *
 * So the structure so far for our gdelData looks like this:
 *
 *                   gdelDataType 4 bytes                                                                         double 8 bytes
 *
 *          |───────────────────────────────────|                                   |───────────────────────────────────────────────────────────────────────|
 *
 *          ╔════════╦════════╦════════╦════════╦════════╦════════╦════════╦════════╦════════╦════════╦════════╦════════╦════════╦════════╦════════╦════════╗
 *          ║        ║        ║        ║        ║        ║        ║        ║        ║   b0   ║   b1   ║   b2   ║   b3   ║   b4   ║   b5   ║   b6   ║   b7   ║               
 *          ╚════════╩════════╩════════╩════════╩════════╩════════╩════════╩════════╩════════╩════════╩════════╩════════╩════════╩════════╩════════╩════════╝
 *
 *                                              |───────────────────────────────────|───────|    
 *
 *                                                              pading 4 bytes       bool 1 byte
 *
 * What is that padding right there? Well, c and c++ compilers like variables to be spaced equially, so as the union is 8 bytes and the gdelDataType only 4, it adds those 4 extra bytes so 
 * everything is alligned.
 *
 *
 * Update: 
 *      Now it also contains a pointer to a register (GDEL version of objects). It is also inside the union, so no more bytes required for this. Same structure as before.
 */
typedef struct {
    gdelDataType type;
    union {
        bool _bool;
        double _num;
        gdelRegister* _reg;
    } data;
} gdelData;


typedef struct {
    size_t maxSize;
    size_t currentDataValue;
    gdelData* pool;
} gdelDataPool;

void initGdelDataPool(gdelDataPool* _dataPool);
void writeToDataPool(gdelDataPool* _dataPool, gdelData _data);
void freeDataPool(gdelDataPool* _dataPool);

bool areGdelDataEqual(gdelData& _left, gdelData& _right);
/*
 * Those macros return a gdelData variable with the corresponding data inside (constants, numbers, true, false...)
 */
#define CREATE_GDEL_BOOL(_data)    ((gdelData){gdelDataType::DT_BOOL, {._bool = _data}})
#define CREATE_GDEL_NULL           ((gdelData){gdelDataType::DT_NULL, {._num = 0}})
#define CREATE_GDEL_NUMBER(_data)  ((gdelData){gdelDataType::DT_NUMBER, {._num = _data}})

#define CREATE_GDEL_REGISTER(_data) ((gdelData){gdelDataType::DT_REGISTER, {._reg = (gdelRegister*)_data}})


/*
 * Those macros take an existing gdelData and returns back the correct union value
 */
#define GET_GDEL_BOOL_DATA(_gdelData)    (_gdelData.data._bool)
#define GET_GDEL_NUMBER_DATA(_gdelData)  (_gdelData.data._num)

#define GET_GDEL_REGISTER_DATA(_gdelData) (_gdelData.data._reg)

/*
 * Those macros take an existing gdelData variable and tells if it is of an specific gdelDataType
 */
#define IS_GDEL_BOOL(_gdelData)    ((_gdelData).type == gdelDataType::DT_BOOL)
#define IS_GDEL_NUMBER(_gdelData)  ((_gdelData).type == gdelDataType::DT_NUMBER)
#define IS_GDEL_NULL(_gdelData)    ((_gdelData).type == gdelDataType::DT_NULL)

#define IS_GDEL_REGISTER(_gdelData) ((_gdelData).type == gdelDataType::DT_REGISTER)
                                            

void printGdelData(const gdelData& _data);

#endif // __VALUES_H__
