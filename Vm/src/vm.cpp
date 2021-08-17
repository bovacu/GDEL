#include "Vm/include/vm.h"
#include "Vm/include/common.h"
#include "Vm/include/debug.h"
#include "Vm/include/compiler.h"
#include <cmath>
#include <iostream>

void gdelVm::init() {
    this->compiler = new gdelCompiler();
    resetStack();
}

gdelProgramResult gdelVm::run(const char* _code) {
    gdelMemBlock _memBlock;
    initGdelMemBlock(&_memBlock);
    
    if (!this->compiler->compile(*this, _code, &_memBlock)) {
        freeGdleMemBlock(&_memBlock);
        std::cerr << "Exit error: " << gdelProgramResult::PROGRAM_COMPILE_ERROR << std::endl;
        return gdelProgramResult::PROGRAM_COMPILE_ERROR;
    }
    
    this->memBlock = &_memBlock;
    this->ip = this->memBlock->byteCode;
    gdelProgramResult result = runGdelVm();
    freeGdleMemBlock(&_memBlock);
    return result;
}

void gdelVm::end() {
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

    #define BINARY_OP(_op)                      \
      do {                                      \
        gdelData _right = popDataFromStack();   \
        gdelData _left = popDataFromStack();    \
        pushDataToStack(_left _op _right);      \
      } while (false)

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
                gdelData _data = popDataFromStack();
                std::cout << _data << std::endl;
                return gdelProgramResult::PROGRAM_OK;
            }

            /*
             * Bytes: 2
             *      - OP_CONST value (0x01)
             *      - Address of the const stored inside the data pool
            */
            case gdelOpCode::OP_CONST: {
                gdelData _const = this->memBlock->dataPool.pool[*this->ip++];
                pushDataToStack(_const);
                break;
            }

            /*
             * Bytes: 1
            */
            case gdelOpCode::OP_ADD:     BINARY_OP(+); break;
            case gdelOpCode::OP_SUB:     BINARY_OP(-); break;
            case gdelOpCode::OP_MUL:     BINARY_OP(*); break;
            case gdelOpCode::OP_DIV:     BINARY_OP(/); break;
            case gdelOpCode::OP_PERCENT: {
                gdelData _right = popDataFromStack(); 
                gdelData _left = popDataFromStack(); 
                pushDataToStack(std::fmod(_left, _right)); 
                break;
            }
            case gdelOpCode::OP_NEGATE : {
                pushDataToStack(-popDataFromStack());
                break;
            }
            case gdelOpCode::OP_POW: {
                gdelData _right = popDataFromStack(); 
                gdelData _left = popDataFromStack(); 
                gdelData _pow = _left; for(auto _i = 0; _i < std::abs(_right) - 1; _i++) _pow *= _left; 
                pushDataToStack(_pow);
                break;
            }

            default:
                std::cout << "To default with: " << (int)_currentInstruction << std::endl;
                break;
        }
    }

    #undef BINARY_OP
}

