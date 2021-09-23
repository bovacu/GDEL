#include <cmath>
#include <sstream>
#include <iostream>
#include <string>

#include "Vm/include/vm.h"
#include "Vm/include/common.h"
#include "Vm/include/debug.h"
#include "Vm/include/compiler.h"
#include "Vm/include/register.h"
#include "Vm/include//memory.h"

#define THROW_GDEL_ERROR(_message)  std::stringstream _error; \
                                    _error << _message;       \
                                    runtimeError(_error.str().c_str())

void gdelVm::init() {
    this->compiler = new gdelCompiler();
    resetStack();
    this->registerPtr = nullptr;
}

gdelProgramResult gdelVm::run(const char* _code) {
    gdelMemBlock _memBlock;
    initGdelMemBlock(&_memBlock);

    if (!this->compiler->compile(*this, _code, &_memBlock)) {
        freeGdleMemBlock(&_memBlock);
        // std::cerr << "Exit error: " << gdelProgramResult::PROGRAM_COMPILE_ERROR << std::endl;
        return gdelProgramResult::PROGRAM_COMPILE_ERROR;
    }

    this->memBlock = &_memBlock;
    this->ip = this->memBlock->byteCode;
    gdelProgramResult result = runGdelVm();
    freeGdleMemBlock(&_memBlock);
    return result;
}

void gdelVm::end() {
    freeRegisters();
    delete this->compiler;
}

void gdelVm::resetStack() {
    this->stackPtr = this->stack;
}

void gdelVm::pushDataToStack(gdelData _data) {
    *this->stackPtr = _data;
    this->stackPtr++;
}

gdelData gdelVm::popDataFromStack() {
    this->stackPtr--;
    return *this->stackPtr;
}

gdelData gdelVm::peek(int _depth) {
    return this->stackPtr[-1 - _depth];
}

gdelStringRegister* gdelVm::findString(const char* _chars, int _length, uint32_t _hash) {
    return this->stringPool.findString(_chars, _length, _hash);
}

void gdelVm::runtimeError(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);
    size_t instruction = this->ip - this->memBlock->byteCode - 1;
    //int line = vm.chunk->lines[instruction];
    //fprintf(stderr, "[line %d] in script\n", line);
    PRINT_LN_ERROR("Runtime error!!");
    resetStack();
  }

#ifdef DEBUG_EXECUTION
    #ifndef RUNTIME_DEBUG_FUNCS
    #define RUNTIME_DEBUG_FUNCS
        void initTables(gdelMemBlock* _memBlock, fort::utf8_table& _blockInfoTable, fort::utf8_table& _byteCodeTable, fort::utf8_table& _dataPoolTable, fort::utf8_table& _stackTable) {
            // ft_set_u8strwid_func(&u8strwid);

            _blockInfoTable << fort::header << "Name";
            _blockInfoTable << "Used bytes";
            _blockInfoTable << "Total bytes" << fort::endr;

            _byteCodeTable << fort::header << "IP";
            _byteCodeTable << "Instruction";
            _byteCodeTable << "Bytes";
            _byteCodeTable << "DataPoolAddrss" << fort::endr;

            _dataPoolTable << fort::header << "Index";
            _dataPoolTable << "Value" << fort::endr;

            _stackTable << fort::header << "Index";
            _stackTable << "Value" << fort::endr;

            _blockInfoTable << "None" << std::to_string(_memBlock->currentByteIndex).c_str() << std::to_string(_memBlock->maxByteSize).c_str() << fort::endr;

            _blockInfoTable.set_border_style(FT_DOUBLE2_STYLE);
            _byteCodeTable.set_border_style(FT_DOUBLE2_STYLE);
            _dataPoolTable.set_border_style(FT_DOUBLE2_STYLE);
            _stackTable.set_border_style(FT_DOUBLE2_STYLE);

        }
        void printDebug(fort::utf8_table* _blockInfoTable, fort::utf8_table* _byteCodeTable, fort::utf8_table* _dataPoolTable, fort::utf8_table* _stcackTable) {
            fort::utf8_table _rootTable;
            _rootTable.set_border_style(FT_EMPTY_STYLE);

            fort::utf8_table _masterStackTable;
            _masterStackTable.set_border_style(FT_EMPTY_STYLE);
            _masterStackTable << fort::header << "Stack";
            _masterStackTable << fort::endr;
            for(auto _c = 0; _c < _masterStackTable.col_count(); _c++){
                _masterStackTable.column(_c).set_cell_text_align(fort::text_align::center);
            }
            // for(int _i = 0; _i < vm.stackPtr - vm.stack; _i++)
            //     *_stcackTable << ip(_i).c_str() << std::to_string(vm.stack[_i]) << fort::endr;

            _masterStackTable << _stcackTable->to_string() << fort::endr;

            fort::utf8_table _masterBlocksTable;
            _masterBlocksTable.set_border_style(FT_EMPTY_STYLE);
            _masterBlocksTable << fort::header << "ByteCode";
            _masterBlocksTable << "DataPool";
            _masterBlocksTable << fort::endr;
            for(auto _c = 0; _c < _masterBlocksTable.col_count(); _c++){
                _masterBlocksTable.column(_c).set_cell_text_align(fort::text_align::center);
            }
            _masterBlocksTable << _byteCodeTable->to_string() << _dataPoolTable->to_string() << fort::endr;

            _rootTable << _masterBlocksTable.to_string() << _masterStackTable.to_string() << fort::endr;

            std::cout << _rootTable.to_string() << std::endl;
        }

        void printStatistics() {
            fort::utf8_table _blocksChart;

        }
    #endif
#endif

/*
 * This is the main method of our virtual machine. It will take each bytecode and execute it as needed. To see the trace generated
 * by the vm, define the macro DEBUG_EXECUTION
 *
 * The BINARY_OP is the way of reusing as much code as possible, by using C/C++ macros. The reason of the do{}while() loop is easier than
 * you thought. We need to be able to encapsulate inside our macro everything we need, and we need a block statement for that. Then why not
 * use a {  } simple block? Well, if we do if(...) BINARY_OP(+); else { } it will translate to if(...) { ... }; else { ... } and that ;
 * is a compilation error. With this dummy do { ... } while(false) we have that safe block we are looking for.
 *
 * Secondly with BINARY_OP, if we want to do 2 + 3, then first the 2 is pushed to the stack (0x01) and then 3 is pushed (0x02), this means
 * the top element is the right operand and the underlaying one is the left one.
*/
gdelProgramResult gdelVm::runGdelVm() {
    this->ip = this->memBlock->byteCode;

    #define BINARY_OP(_gdelDataType, _op)                                                                           \
       if(!IS_GDEL_NUMBER(peek(0)) || !IS_GDEL_NUMBER(peek(1))) {                                                   \
            THROW_GDEL_ERROR( "Error while binary operation, both operands must be numbers and got gdelDataType '"  \
                              << std::to_string((int)peek(0).type) << "' and gdelDataType '"                        \
                              << std::to_string((int)peek(1).type) << "'");                                         \
            return gdelProgramResult::PROGRAM_RUNTIME_ERROR;                                                        \
        }                                                                                                           \
                                                                                                                    \
        do {                                                                                                        \
            double _right = GET_GDEL_NUMBER_DATA(popDataFromStack());                                               \
            double _left = GET_GDEL_NUMBER_DATA(popDataFromStack());                                                \
            pushDataToStack(_gdelDataType(_left _op _right));                                                       \
        } while (false)

    #define READ_CONSTANT() (this->memBlock->dataPool.pool[*this->ip++])
    #define READ_STRING() (GET_GDEL_STRING(READ_CONSTANT()))

    #ifdef DEBUG_EXECUTION
        fort::utf8_table _blockInfoTable;
        fort::utf8_table _byteCodeTable;
        fort::utf8_table _dataPoolTable;
        fort::utf8_table _stackTable;
        initTables(this->memBlock, _blockInfoTable, _byteCodeTable, _dataPoolTable, _stackTable);
    #endif

    for(;;) {
        #ifdef DEBUG_EXECUTION
            disassembleGdelInstruction(this->memBlock, this->ip - this->memBlock->byteCode, _byteCodeTable, _dataPoolTable);
        #endif

        uint8_t _currentInstruction = *this->ip++; // This first gets the deferenced value of the pointer and then increases the pointer
        switch ((int)_currentInstruction) {
            /*
             * Bytes: 1
            */
            case gdelOpCode::OP_RETURN: {
                #ifdef DEBUG_EXECUTION
                    printDebug(&_blockInfoTable, &_byteCodeTable, &_dataPoolTable, &_stackTable);
                #endif
                // gdelData _data = popDataFromStack();
                // printGdelData(_data);
                return gdelProgramResult::PROGRAM_OK;
            }

            /*
             * Bytes: 2
             *      - OP_CONST value (0x01)
             *      - Address of the const stored inside the data pool
            */
            case gdelOpCode::OP_CONST: {
                gdelData _const = READ_CONSTANT();
                pushDataToStack(_const);
                break;
            }

            /*
             * Bytes: 1
            */
            case gdelOpCode::OP_FALSE: pushDataToStack(CREATE_GDEL_BOOL(false));    break;
            case gdelOpCode::OP_NULL:  pushDataToStack(CREATE_GDEL_NULL);           break;
            case gdelOpCode::OP_TRUE:  pushDataToStack(CREATE_GDEL_BOOL(true));     break;

            /*
             * Bytes: 1
            */
            case gdelOpCode::OP_ADD: {
                auto _peekRight = peek(0);
                auto _peekLeft = peek(1);

                if(IS_GDEL_STRING(_peekLeft) && IS_GDEL_STRING(_peekRight)) {
                    concatGdelStrings();
                } else if(IS_GDEL_NUMBER(_peekLeft) && IS_GDEL_NUMBER(_peekRight)) {
                    BINARY_OP(CREATE_GDEL_NUMBER, +); break;
                } else {
                    THROW_GDEL_ERROR("Error adding type '" << std::to_string((int)_peekLeft.type) << "' and type '" << std::to_string((int)_peekRight.type) << "'");
                    return gdelProgramResult::PROGRAM_RUNTIME_ERROR;
                }

                break;
            }
            case gdelOpCode::OP_SUB:     BINARY_OP(CREATE_GDEL_NUMBER, -); break;
            case gdelOpCode::OP_MUL:     BINARY_OP(CREATE_GDEL_NUMBER, *); break;
            case gdelOpCode::OP_DIV:     BINARY_OP(CREATE_GDEL_NUMBER, /); break;
            case gdelOpCode::OP_PERCENT: {
                auto _right = GET_GDEL_NUMBER_DATA(popDataFromStack());
                auto _left = GET_GDEL_NUMBER_DATA(popDataFromStack());
                pushDataToStack(CREATE_GDEL_NUMBER(std::fmod(_left, _right)));
                break;
            }
            case gdelOpCode::OP_POW: {
                auto _right = GET_GDEL_NUMBER_DATA(popDataFromStack());
                auto _left = GET_GDEL_NUMBER_DATA(popDataFromStack());
                auto _pow = _left;
                for(auto _i = 0; _i < std::abs(_right) - 1; _i++) _pow *= _left;
                pushDataToStack(CREATE_GDEL_NUMBER(_pow));
                break;
            }

            /*
             * Bytes: 1
            */
            case gdelOpCode::OP_NOT:
            case gdelOpCode::OP_NEGATE : {
                auto _peek = peek(0);
                if(IS_GDEL_NUMBER(_peek)) {
                    pushDataToStack(CREATE_GDEL_NUMBER(-GET_GDEL_NUMBER_DATA(popDataFromStack())));
                } else if(IS_GDEL_BOOL(_peek) || IS_GDEL_NULL(_peek)) {
                    auto _notValue = IS_GDEL_NULL(_peek) || (IS_GDEL_BOOL(_peek) && !GET_GDEL_BOOL_DATA(_peek));
                    pushDataToStack(CREATE_GDEL_BOOL(_notValue));
                } else {
                    THROW_GDEL_ERROR("Error while negating, operands must be bool or number but got: gdelDataType '" << std::to_string((int)peek(0).type) << "'");
                    return gdelProgramResult::PROGRAM_RUNTIME_ERROR;
                }
                break;
            }

            case gdelOpCode::OP_EQUAL: {
                gdelData _left = popDataFromStack();
                gdelData _right = popDataFromStack();
                pushDataToStack(CREATE_GDEL_BOOL(areGdelDataEqual(_left, _right)));
                break;
            }
            case gdelOpCode::OP_GREAT: {
                auto _peekRight = peek(0);
                auto _peekLeft = peek(1);
                if(IS_GDEL_STRING(_peekLeft) && IS_GDEL_STRING(_peekRight)) {
                    pushDataToStack(CREATE_GDEL_BOOL(GET_GDEL_STRING(_peekLeft)->length > GET_GDEL_STRING(_peekRight)->length));
                } else if(IS_GDEL_NUMBER(_peekLeft) && IS_GDEL_NUMBER(_peekRight)) {
                    BINARY_OP(CREATE_GDEL_BOOL, >); 
                } else {
                    THROW_GDEL_ERROR("Error: comparisson >, <, >=, <= can only be used with comparable values, not type '" << _peekLeft.type << "' and '" << _peekRight.type << "'");
                    return gdelProgramResult::PROGRAM_RUNTIME_ERROR;
                }

                break;
            }
            case gdelOpCode::OP_LESS: {
                auto _peekRight = peek(0);
                auto _peekLeft = peek(1);
                if(IS_GDEL_STRING(_peekLeft) && IS_GDEL_STRING(_peekRight)) {
                    pushDataToStack(CREATE_GDEL_BOOL(GET_GDEL_STRING(_peekLeft)->length < GET_GDEL_STRING(_peekRight)->length));
                } else if(IS_GDEL_NUMBER(_peekLeft) && IS_GDEL_NUMBER(_peekRight)) {
                    BINARY_OP(CREATE_GDEL_BOOL, <); 
                } else {
                    THROW_GDEL_ERROR("Error: comparisson >, <, >=, <= can only be used with comparable values, not type '" << _peekLeft.type << "' and '" << _peekRight.type << "'");
                    return gdelProgramResult::PROGRAM_RUNTIME_ERROR;
                }

                break;
            }
            
            /*
             * Bytes: 1
            */
            case gdelOpCode::OP_PRINT:  printGdelData(popDataFromStack()); break;
            case gdelOpCode::OP_POP:    popDataFromStack(); break;

            case gdelOpCode::OP_DEFINE_GLOBAL_VAR: {
                gdelStringRegister* _name = READ_STRING();
                this->globalVars.addEntry(_name, peek(0));
                popDataFromStack();
                break;
            }

            case gdelOpCode::OP_GET_GLOBAL_VAR: {
                gdelStringRegister* _name = READ_STRING();
                gdelData* _data = this->globalVars.getEntry(_name);
                if(!_data) {
                    THROW_GDEL_ERROR("Error: undefined variable '" << _name->characters <<"'");
                    return gdelProgramResult::PROGRAM_RUNTIME_ERROR;
                }

                pushDataToStack(*_data);
                break;
            }

            default:
                std::cout << "To default with: " << (int)_currentInstruction << std::endl;
                break;

        }
    }

    #undef BINARY_OP
}

void gdelVm::concatGdelStrings() {
    gdelStringRegister* _leftString = GET_GDEL_STRING(popDataFromStack());
    gdelStringRegister* _rightString = GET_GDEL_STRING(popDataFromStack());
    int _length = _leftString->length + _rightString->length;
    char* _heapChars = GDEL_ALLOCATE_HEAP(char, _length + 1);
    strcpy(_heapChars, _rightString->characters);
    strcat(_heapChars, _leftString->characters);
    _heapChars[_length] = '\0';

 
    gdelStringRegister* _result = takeString(*this, _heapChars, _length);
    pushDataToStack(CREATE_GDEL_REGISTER(_result));
}

void gdelVm::freeRegister(gdelRegister* _reg) {
    switch (_reg->type) {
        case gdelRegisterType::REG_STRING: {
            gdelStringRegister* _stringReg = (gdelStringRegister*)_reg;
            GDEL_FREE_BLOCK(char, _stringReg->characters, _stringReg->length + 1);
            GDEL_FREE_HEAP(gdelRegister, _reg);
            break;
        }
    }
}

void gdelVm::freeRegisters() {
    gdelRegister* _reg = this->registerPtr;
    while (_reg != nullptr) {
        gdelRegister* _next = _reg->nextRegister;
        freeRegister(_reg);
        _reg = _next;
    } 
}

bool gdelVm::isAnyType(const gdelData& _data, gdelDataType _types[]) {
    auto _size = sizeof(*_types) / sizeof(_types[0]);
    bool _is = false;
    for(auto _i = 0; _i < _size; _i++)
        _is = _types[_i] | _data.type;
    
    return _is;
}

void gdelVm::addStringToPool(gdelStringRegister* _string) {
    stringPool.addEntry(_string, CREATE_GDEL_NULL);
}