#include "Vm/include/debug.h"
#include "Vm/include/register.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

#pragma region WRITE_TO_TABLE_METHODS
	/*
	* Just for rendering correctly the emojis
	*/
	#if defined(FT_HAVE_UTF8)
	/* Custom function to compute visible width of utf8 strings */
	int u8strwid(const void *beg, const void *end, size_t *width)
	{
		const char *emojis[] = {"🟢", "🔴"};
		const size_t sz = sizeof(emojis) / sizeof(emojis[0]);
		const size_t raw_len = (const char *)end - (const char *)beg;

		for (size_t i = 0; i < sz; ++i) {
			if (memcmp(beg, emojis[i], std::min(strlen(emojis[i]), raw_len)) == 0) {
				*width = 2; /* On my terminal emojis have width of 2 chars */
				return 0;
			}
		}
		return 1;
	}
	#endif

	std::string int_to_hex(int _op){
	std::stringstream stream;
	stream << "0x"
			<< std::setfill ('0') << std::setw(sizeof(uint8_t)*2)
			<< std::hex << _op;
	return stream.str();
	}

	std::string ip(int _op){
		std::stringstream stream;
		stream  << std::setfill ('0') << std::setw(5) << _op;
		return stream.str();
	}
#pragma endregion WRITE_TO_TABLE_METHODS

/*
 * This method just prints the current instruction and advances the instruction pointer. This is only useful when executing
 * instructions that takes 1 byte, the instruction itself as other instructions take more bytes and more positions would be
*/
static size_t simpleInstruction(gdelMemBlock* _memBlock, const char* _instructionName, size_t _byteCodeArrIndex, fort::utf8_table& _byteCodeTable) {
    std::string _val(_instructionName);
    _val.append(" ("); _val.append(int_to_hex(_memBlock->byteCode[_byteCodeArrIndex])); _val.append(")");
    _byteCodeTable << ip(_byteCodeArrIndex).c_str() << _val.c_str() << "1" << "─────" << fort::endr;
    return _byteCodeArrIndex + 1;
}

/*
 * This instruction takes two bytes, the first one is the OP_CONST which tells the computer to look for a const value in the data
 * pool and the next byte is the pointer of the address to the data pool where the value is stored.
*/
static size_t constantInstruction(gdelMemBlock* _memBlock, const char* _instructionName, size_t _byteCodeArrIndex, fort::utf8_table& _byteCodeTable, fort::utf8_table& _dataPoolTable) {
	uint8_t _constantAddrss = _memBlock->byteCode[_byteCodeArrIndex + 1]; //_byteCodeArrIndex is the OP_CONST and right after is the value, that's why the +1
	gdelData _data = _memBlock->dataPool.pool[_constantAddrss];
	std::string _val(_instructionName);
    _val.append(" ("); _val.append(int_to_hex(_memBlock->byteCode[_byteCodeArrIndex])); _val.append(")");
    _byteCodeTable << ip(_byteCodeArrIndex).c_str() << _val.c_str() << "2" << ip(_constantAddrss).c_str() << fort::endr;

	std::string _dataStr;
	switch(_data.type) {
		case gdelDataType::DT_NUMBER: _dataStr = std::to_string(GET_GDEL_NUMBER_DATA(_data)); break;
		case gdelDataType::DT_NULL: _dataStr = "null"; break;
		case gdelDataType::DT_BOOL: GET_GDEL_BOOL_DATA(_data) ? _dataStr = "true" : _dataStr = "false";
		case gdelDataType::DT_REGISTER: {
			switch (GET_GDEL_REGISTER_TYPE(_data)) {
			case gdelRegisterType::REG_STRING:
				_dataStr = "'";
				_dataStr.append(GET_GDEL_PTR_STRING(_data));
				_dataStr.push_back('\'');
				break;
    		}
		};
	}

    _dataPoolTable << ip(_constantAddrss).c_str() << _dataStr.c_str() << fort::endr;
    return _byteCodeArrIndex + 2;
}

/*
 * Take every bytecode inside a memblock and print it, side-to-side with its IP, Instruction name, reference to DataPool and Status
*/
void disassembleGdelMemBlock(gdelMemBlock* _memBlock, const char* _memBlockName) {
    ft_set_u8strwid_func(&u8strwid);

    fort::utf8_table _blockInfoTable;
    ADD_HEADER(_blockInfoTable, "   Name   ", false);
    ADD_HEADER(_blockInfoTable, "   Used bytes   ", false);
    ADD_HEADER(_blockInfoTable, "   Total bytes  ", true);
    ADD_ROW(_blockInfoTable, _memBlockName, std::to_string(_memBlock->currentByteIndex).c_str(), std::to_string(_memBlock->maxByteSize).c_str());

	fort::utf8_table _byteCodeTable;
	ADD_HEADER(_byteCodeTable, "   IP   ",          false);
    ADD_HEADER(_byteCodeTable, "   Instruction   ", false);
    ADD_HEADER(_byteCodeTable, "   DataPoolAddrss   ", false);
    ADD_HEADER(_byteCodeTable, "   Status   ",      true);

	fort::utf8_table _dataPoolTable;
	ADD_HEADER(_dataPoolTable, "   Index   ", false);
    ADD_HEADER(_dataPoolTable, "   Value   ", true);

    size_t _byteArrIndex = 0;
    while(_byteArrIndex < _memBlock->currentByteIndex)
        _byteArrIndex = disassembleGdelInstruction(_memBlock, _byteArrIndex, _byteCodeTable, _dataPoolTable);

    TABLE_STYLE(_blockInfoTable);
    TABLE_STYLE(_byteCodeTable);
    TABLE_STYLE(_dataPoolTable);
    std::cout << _blockInfoTable.to_string() << _byteCodeTable.to_string() << _dataPoolTable.to_string() << std::endl;
}

size_t disassembleGdelInstruction(gdelMemBlock* _memBlock, size_t _byteCodeArrIndex, fort::utf8_table& _byteCodeTable, fort::utf8_table& _dataPoolTable) {
    uint8_t _currentInstruction = _memBlock->byteCode[_byteCodeArrIndex];
    switch (_currentInstruction) {
      	case gdelOpCode::OP_RETURN:
        	return simpleInstruction(_memBlock, "OP_RETURN", _byteCodeArrIndex, _byteCodeTable);
      	case OP_CONST:
        	return constantInstruction(_memBlock, "OP_CONSTANT", _byteCodeArrIndex, _byteCodeTable, _dataPoolTable);
		case OP_NEGATE:
			return simpleInstruction(_memBlock, "OP_NEGATE", _byteCodeArrIndex, _byteCodeTable);
		case OP_ADD:
			return simpleInstruction(_memBlock, "OP_ADD", _byteCodeArrIndex, _byteCodeTable);
		case OP_SUB:
			return simpleInstruction(_memBlock, "OP_SUB", _byteCodeArrIndex, _byteCodeTable);
		case OP_MUL:
			return simpleInstruction(_memBlock, "OP_MUL", _byteCodeArrIndex, _byteCodeTable);
		case OP_DIV:
			return simpleInstruction(_memBlock, "OP_DIV", _byteCodeArrIndex, _byteCodeTable);
		case OP_POW:
			return simpleInstruction(_memBlock, "OP_POW", _byteCodeArrIndex, _byteCodeTable);
		case OP_PERCENT:
			return simpleInstruction(_memBlock, "OP_MOD", _byteCodeArrIndex, _byteCodeTable);
		case OP_NULL:
        	return simpleInstruction(_memBlock, "OP_NULL", _byteCodeArrIndex, _byteCodeTable);
      	case OP_TRUE:
        	return simpleInstruction(_memBlock, "OP_TRUE", _byteCodeArrIndex, _byteCodeTable);
      	case OP_FALSE:
        	return simpleInstruction(_memBlock, "OP_FALSE", _byteCodeArrIndex, _byteCodeTable);
		case OP_NOT:
        	return simpleInstruction(_memBlock, "OP_NOT", _byteCodeArrIndex, _byteCodeTable);
		case OP_EQUAL:
        	return simpleInstruction(_memBlock, "OP_EQUAL", _byteCodeArrIndex, _byteCodeTable);
		case OP_GREAT:
        	return simpleInstruction(_memBlock, "OP_GREAT", _byteCodeArrIndex, _byteCodeTable);
		case OP_LESS:
        	return simpleInstruction(_memBlock, "OP_LESS", _byteCodeArrIndex, _byteCodeTable);
      	default:
			std::string _val("Unknown");
			_val.append(" ("); _val.append(int_to_hex(_currentInstruction)); _val.append(")");
			_byteCodeTable << ip(_byteCodeArrIndex).c_str() << _val.c_str() << "0" << fort::endr;
			return _byteCodeArrIndex + 1;
    }
}
