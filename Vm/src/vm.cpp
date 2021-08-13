#include "Vm/include/vm.h"
#include "Vm/include/common.h"
#include "Vm/include/debug.h"
#include <iostream>

gdelVm vm;

void initGdelVm() {

}

void freeGdelVm() {

}

#ifdef DEBUG_EXECUTION
    #ifndef RUNTIME_DEBUG_FUNCS
    #define RUNTIME_DEBUG_FUNCS
        void initTables(gdelMemBlock* _memBlock, fort::utf8_table& _blockInfoTable, fort::utf8_table& _byteCodeTable, fort::utf8_table& _dataPoolTable) {
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

            _blockInfoTable << "None" << std::to_string(_memBlock->currentByteIndex).c_str() << std::to_string(_memBlock->maxByteSize).c_str() << fort::endr;

            _blockInfoTable.set_border_style(FT_DOUBLE2_STYLE);
            _byteCodeTable.set_border_style(FT_DOUBLE2_STYLE);
            _dataPoolTable.set_border_style(FT_DOUBLE2_STYLE);
        }

        void printDebug(fort::utf8_table* _blockInfoTable, fort::utf8_table* _byteCodeTable, fort::utf8_table* _dataPoolTable) {
            fort::utf8_table _masterTable;
            _masterTable.set_border_style(FT_EMPTY_STYLE);
            _masterTable.cur_cell().set_cell_text_style(fort::text_style::bold);
            _masterTable.cur_cell().set_cell_content_fg_color(fort::color::light_yellow);
            _masterTable << fort::header << "ByteCode";
            _masterTable.cur_cell().set_cell_text_style(fort::text_style::bold);
            _masterTable.cur_cell().set_cell_content_fg_color(fort::color::light_yellow);
            _masterTable << "DataPool";
            _masterTable.cur_cell().set_cell_text_style(fort::text_style::bold);
            _masterTable.cur_cell().set_cell_content_fg_color(fort::color::light_yellow);
            _masterTable << "Info";
            _masterTable.cur_cell().set_cell_text_style(fort::text_style::bold);
            _masterTable.cur_cell().set_cell_content_fg_color(fort::color::light_yellow);
            _masterTable << fort::endr;
            for(auto _c = 0; _c < _masterTable.col_count(); _c++){
                _masterTable.column(_c).set_cell_text_align(fort::text_align::center);
            }
            _masterTable << _byteCodeTable->to_string() << _dataPoolTable->to_string() << _blockInfoTable->to_string() << fort::endr;
            std::cout << _masterTable.to_string() << std::endl;
        }
    #endif
#endif

/*
 * This is the main method of our virtual machine. It will take each bytecode and execute it as needed. To see the trace generated
 * by the vm, define the macro DEBUG_EXECUTION
*/
gdelProgramResult runGdelVm(gdelMemBlock* _memBlock) {
    vm.memBlock = _memBlock;
    vm.ip = _memBlock->byteCode;

    #ifdef DEBUG_EXECUTION
        fort::utf8_table _blockInfoTable;
        fort::utf8_table _byteCodeTable;
        fort::utf8_table _dataPoolTable;
        initTables(vm.memBlock, _blockInfoTable, _byteCodeTable, _dataPoolTable);
    #endif
    for(;;) {
        #ifdef DEBUG_EXECUTION
            disassembleGdelInstruction(vm.memBlock, (vm.ip - vm.memBlock->byteCode), _byteCodeTable, _dataPoolTable);
        #endif

        uint8_t _currentInstruction = *vm.ip++;
        switch ((int)_currentInstruction) {
            /*
             * Bytes: 1
            */
            case gdelOpCode::OP_RETURN: {
                #ifdef DEBUG_EXECUTION
                    printDebug(&_blockInfoTable, &_byteCodeTable, &_dataPoolTable);
                #endif
                return gdelProgramResult::PROGRAM_OK;
            }

            /*
             * Bytes: 2
             *      - OP_CONST value (0x01)
             *      - Address of the const stored inside the data pool
            */
            case gdelOpCode::OP_CONST: {
                gdelData _const = vm.memBlock->dataPool.pool[*vm.ip++];
                break;
            }

            default:
                std::cout << "To default with: " << (int)_currentInstruction << std::endl;
                break;
        }
    }
}

