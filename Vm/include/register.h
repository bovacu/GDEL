#ifndef __REGISTER__
#define __REGISTER__

#include "Vm/include/common.h"
#include "Vm/include/data.h"

class gdelVm;

/*
 * This class represents the OOP objects, but in GDEL are called registers. gdelRegister is the base for every other kind of object that is defined
 * in the language. For example, gdelStringRegister is a gdelRegister with added attributes.
 *
 *
*/

/*
 *  +---------------------------------------------------------------+
 *  |                     VERY VERY IMPORTANT                       |
 *  +---------------------------------------------------------------+
 *  |                                                               |
 *  | In C and C++ the order of the attributes is really important. |
 *  | Variables are stored in the same order as declared, and as    |
 *  | stated inside data.h, C/C++ compilers like to give even       |
 *  | padding to variables, so if we set double, int double,        |
 *  | There will be a padding between the first two variables,      |
 *  | but if we do double, double, int, there won't be and we       |
 *  | be wasting memory.                                            |
 *  |                                                               | 
 *  | By using inheritance, the order for the child will be the     |
 *  | order of the father and then the order of the child           |
 *  |                                                               |
 *  +---------------------------------------------------------------+
*/

typedef enum {
    REG_STRING
} gdelRegisterType;

struct gdelRegister {
    gdelRegisterType type;
    struct gdelRegister* nextRegister;
};

struct gdelStringRegister : gdelRegister {
    int length;
    char* characters; // Heap allocated so we don't have to worry about it's size
    uint32_t hash;
};

#define GET_GDEL_REGISTER_TYPE(_register) (GET_GDEL_REGISTER_DATA(_register)->type)

/*
 * Whenever a variable is passed to a macro, the variable is evaluated as many times as it is refered. That means, depending on the situation, it can
 * do multiple times and operation that we don't want it to do. If we don't use the function and pass the code of the function to the macro and do 
 * something like IS_GDEL_STRING(POP()), we will pop two variables as it expands to IS_GDEL_REGISTER(POP()) && GET_GDEL_REGISTER_DATA(POP())->type == _regType;
*/
inline bool isRegisterType(gdelData _data, gdelRegisterType _regType){
    return IS_GDEL_REGISTER(_data) && GET_GDEL_REGISTER_DATA(_data)->type == _regType;
}

gdelStringRegister* copyString(gdelVm& _vm, const char* _chars, int _length);
gdelStringRegister* takeString(gdelVm& _vm, char* _chars, int _length);
void printRegister(const gdelData& _data);

#define IS_GDEL_STRING(_gdelData) isRegisterType(_gdelData, gdelRegisterType::REG_STRING)
#define GET_GDEL_STRING(_gdelData) ((gdelStringRegister*)GET_GDEL_REGISTER_DATA(_gdelData))
#define GET_GDEL_PTR_STRING(_gdelData) (((gdelStringRegister*)GET_GDEL_REGISTER_DATA(_gdelData))->characters)

#endif // __REGISTER__