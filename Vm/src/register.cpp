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

inline gdelStringRegister* allocateString(gdelVm& _vm, char* chars, int length, uint32_t _hash) {
	gdelStringRegister* _stringReg = GDEL_ALLOCATE_REGISTER_HEAP(_vm, gdelStringRegister, gdelRegisterType::REG_STRING);
	_stringReg->length = length;
	_stringReg->characters = chars;
	_stringReg->hash = _hash;
    _vm.addStringToPool(_stringReg);
	return _stringReg;
}

uint32_t getHash(const char* _key, int _length) {
	uint32_t hash = 2166136261u;
	for (int i = 0; i < _length; i++) {
		hash ^= (uint8_t)_key[i];
		hash *= 16777619;
	}
	return hash;
}

gdelStringRegister* copyString(gdelVm& _vm, const char* _chars, int _length) {
    uint32_t _hash = getHash(_chars, _length);
    gdelStringRegister* _interned = _vm.findString(_chars, _length, _hash);

    if(_interned != nullptr) return _interned;
    char* _heapChars = GDEL_ALLOCATE_HEAP(char, _length + 1);
    memcpy(_heapChars, _chars, _length);
    _heapChars[_length] = '\0';
    return allocateString(_vm, _heapChars, _length, _hash);
}

gdelStringRegister* takeString(gdelVm& _vm, char* _chars, int _length) {
    uint32_t _hash = getHash(_chars, _length);

    gdelStringRegister* _interned = _vm.findString(_chars, _length, _hash);
    if(_interned != nullptr) {
        GDEL_FREE_BLOCK(char, _chars, _length + 1); // +1 is for the null termination
        return _interned;
    }

	return allocateString(_vm, _chars, _length, _hash);
}

void printRegister(const gdelData& _data) {
    switch (GET_GDEL_REGISTER_TYPE(_data)) {
      case gdelRegisterType::REG_STRING:
        std::cout << GET_GDEL_PTR_STRING(_data) << std::endl;
        break;
    }
}