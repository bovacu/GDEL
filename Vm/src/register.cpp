#include "Vm/include/register.h"
#include "Vm/include/memory.h"
#include "Vm/include/vm.h"

inline gdelRegister* allocateRegister(gdelVm& _vm, size_t _size, gdelRegisterType _type) {
    gdelRegister* _reg = (gdelRegister*)reallocate(NULL, 0, _size);
    _reg->type = _type;
    _reg->nextRegister = _vm.registerPtr;
    _vm.registerPtr = _reg;
    return _reg;
}

#define GDEL_ALLOCATE_REGISTER_HEAP(_vm, _type, _regType) \
      (_type*)allocateRegister(_vm, sizeof(_type), _regType)

inline gdelStringRegister* allocateString(gdelVm& _vm, char* chars, int length) {
    gdelStringRegister* _stringReg = GDEL_ALLOCATE_REGISTER_HEAP(_vm, gdelStringRegister, gdelRegisterType::REG_STRING);
    _stringReg->length = length;
    _stringReg->characters = chars;
    return _stringReg;
  }

gdelStringRegister* copyString(gdelVm& _vm, const char* _chars, int _length) {
    char* heapChars = GDEL_ALLOCATE_HEAP(char, _length + 1);
    memcpy(heapChars, _chars, _length);
    heapChars[_length] = '\0';
    return allocateString(_vm, heapChars, _length);
}

gdelStringRegister* takeString(gdelVm& _vm, char* _chars, int _length) {
	return allocateString(_vm, _chars, _length);
}

void printRegister(const gdelData& _data) {
    switch (GET_GDEL_REGISTER_TYPE(_data)) {
      case gdelRegisterType::REG_STRING:
        std::cout << GET_GDEL_PTR_STRING(_data) << std::endl;
        break;
    }
}