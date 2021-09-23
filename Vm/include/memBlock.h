#ifndef __MEMBLOCK_H__
#define __MEMBLOCK_H__

#include "common.h"
#include "data.h"

/*
 * The memBlock is structured like the next scheme:
 *
 *          gdelMemBlock                                                        gdelDataPool
 *      ╔═════════════════╗                                                 ╔═════════════════╗
 *      ║   ───────────   ║ < gdelMemBlock::maxByteSize = 8                 ║   ───────────   ║ < gdelDataPool::maxSize = 8
 *      ╠═════════════════╣                                                 ╠═════════════════╣
 *      ║   ───────────   ║                                                 ║   ───────────   ║
 *      ╠═════════════════╣                                                 ╠═════════════════╣
 *      ║   ───────────   ║                                                 ║   ───────────   ║
 *      ╠═════════════════╣                                                 ╠═════════════════╣
 *      ║   ───────────   ║                                                 ║   ───────────   ║
 *      ╠═════════════════╣                                                 ╠═════════════════╣
 *      ║      OP_X       ║ < gdelMemBlock::currentByteIndex = 5            ║   ───────────   ║
 *      ╠═════════════════╣                                                 ╠═════════════════╣
 *      ║      OP_Y       ║                                                 ║   ───────────   ║
 *      ╠═════════════════╣    Value: currentDataValue - 1                  ╠═════════════════╣
 *      ║ OP_Y_DATA_INDEX ║  ─────────────────────────────────┐             ║   ───────────   ║
 *      ╠═════════════════╣                                   │             ╠═════════════════╣  
 *      ║      OP_T       ║                                   │             ║   ───────────   ║
 *      ╠═════════════════╣                                   │             ╠═════════════════╣
 *      ║      OP_S       ║                                   └───────────> ║     CONST_Y     ║ < gdelDataPool::currentDataValue = 1
 *      ╚═════════════════╝                                                 ╚═════════════════╝
 * 
 * 
 * 
 * 
*/

/* This enum contains all the instructions that our VM will be able to run, in other words, the bytecode our VM will execute.
 * Operations will const of 1 or 2 parts, depending on if they are simple or not.
 * 
 * Simple: [ 1byte ]        complex: [ 1 byte ][ 1 byte ]
 *             ^                          ^         ^
 *          operand                    operand  dataIndex -> to look inside gdelMemBlock->dataPool
*/
typedef enum {
    OP_RETURN            = 0x00,
    OP_CONST             = 0x01,
    OP_NEGATE            = 0x02,
    OP_ADD               = 0x03,
    OP_SUB               = 0x04,
    OP_MUL               = 0x05,
    OP_DIV               = 0x06,
    OP_PERCENT           = 0x07,
    OP_POW               = 0x08,
    OP_TRUE              = 0x09,
    OP_FALSE             = 0x10,
    OP_NULL              = 0x11,
    OP_NOT               = 0x12,
    OP_EQUAL             = 0x13,
    OP_GREAT             = 0x14,
    OP_LESS              = 0x15,
    OP_PRINT             = 0x16,
    OP_POP               = 0x17,
    OP_DEFINE_GLOBAL_VAR = 0x018,
    OP_GET_GLOBAL_VAR    = 0x019,
    TEST_ERROR           = 0x99
} gdelOpCode;

/*  This is the base for instruction allocation of our VM. We will store the instructions as a dynamic array of bytes:
        - Dynamic arrays are cache-friendly (locality of references).
        - Constant time indexing and appending (this last one at the end of the array)
    
    Our array of bytecode will have X capacity, which will increase or decrease depending on the program's needs, but the structure
    will be like this:

    gdelMemblock -> [ byte1 | byte2 | byte3 | --- | --- | --- | --- | --- ]
                                        ^                              ^
                                currentByteIndex = 3              maxByteSize = 8
*/
typedef struct {
    size_t currentByteIndex;   // position of the last byte stored inside the dynamic array
    size_t maxByteSize;        // amount of total available spaces in the dynamic array
    byte* byteCode;            // remember that Byte is a typedef for unint8_t declared in common.h
    gdelDataPool dataPool;     // this is mainly for the constants and values calculated on this memBlock to be stored
} gdelMemBlock;


void initGdelMemBlock(gdelMemBlock* _memBlock);
void writeToGdelMemBlock(gdelMemBlock* _memBlock, byte _byte);
size_t writeConstantToGdelMemBlock(gdelMemBlock* _memBlock, gdelData _data);
void freeGdleMemBlock(gdelMemBlock* _memBlock);

#endif // __MEMBLOCK_H__
