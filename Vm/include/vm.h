#ifndef __VM_H__
#define __VM_H__

#include "Vm/include/memBlock.h"
#include "Vm/include/common.h"

#define STACK_SIZE 256

/*
 * This struct is the heart everyting. It contains the memBlock to run at each time and the IP.
 * The IP is the instruction pointer (or PC, program counter) and it is a pointer that points to the operation that is going to be
 * execute next, not the one that is currently happening.
 *
 * It also contains the stack, fixed-sized array of gdelData. This stack is used to store the information declared on each function.
 * The stack pointer always points to one element up the highest element, like this:
 *
 *
 *      ╔═   ╔═════════════════╗
 *      ║    ║   ───────────   ║ < gdelVm stack top = 255
 *      ║    ╠═════════════════╣
 *      ║    ║   ───────────   ║
 *      ║    ╠═════════════════╣
 *      ║    ║      ....       ║
 * 256b ║    ║      ....       ║
 *      ║    ║      ....       ║
 *      ║    ╠═════════════════╣
 *      ║    ║   ───────────   ║ < gdelVm::stackPtr = 2
 *      ║    ╠═════════════════╣
 *      ║    ║        Z        ║
 *      ║    ╠═════════════════╣
 *      ║    ║        C        ║ < gdelVm stack bottom
 *      ╚═   ╚═════════════════╝
 *
 * Our stack will pop and push values as needed and to. Pointing to an upper element than the highest means that if our stackPtr is
 * 0, the stack is empty. For now, our stack is fixed, what means it can produce a stack-overflow error, but this will be change to be
 * a dynamic array.
*/

typedef enum {
    PROGRAM_OK,
    PROGRAM_COMPILE_ERROR = -1,
    PROGRAM_RUNTIME_ERROR = -2
} gdelProgramResult;

class gdelCompiler;
class gdelVm {
    private:
        byte* ip;
        gdelMemBlock* memBlock;
        gdelData stack[STACK_SIZE];
        gdelData* stackPtr;

        gdelCompiler* compiler;

    public:
        void init();
        void end();
        gdelProgramResult run(const char* _code);

    private:
        gdelProgramResult runGdelVm();
        void resetStack();
        void pushDataToStack(gdelData _data);
        gdelData popDataFromStack();
        gdelData peek(int _depth);
        void runtimeError(const char* _error, ...);
};

#endif // __VM_H__
