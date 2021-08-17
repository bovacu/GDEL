#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "Vm/include/memBlock.h"
#include "fort.hpp"
#include <string.h>
#include <sstream>
#include <stdarg.h>
#include <iostream>

/*
 * Both the enum and the u8strwid is used to render correctly the table of instructions when debugging
*/
typedef enum {
    // ERROR = -1,
    OK = 0
} gdelOpStatus;
int u8strwid(const void *beg, const void *end, size_t *width);

#define TABLE_STYLE(_table)  _table.set_border_style(FT_DOUBLE2_STYLE);
                           

#define ADD_HEADER(_table, _header, _end)   _table << fort::header << _header;                                      \
                                            if(_end) _table << fort::endr;                                          \
                                          
template<typename ...Args>
constexpr std::size_t va_count(Args&&...) { return sizeof...(Args); }

#define ADD_ROW(_table, ...) [&](fort::utf8_table& _t, int _foo, ...)  {      \
                                    int _numArgs = va_count(__VA_ARGS__);     \
                                    va_list _list;                            \
                                    va_start(_list, _foo);                     \
                                    while (_numArgs--)                        \
                                        _t << va_arg(_list, const char*);     \
                                    va_end(_list);                            \
                                    _t << fort::endr;                         \
                                    }(_table, 0, __VA_ARGS__)

std::string ip(int _op);

void disassembleGdelMemBlock(gdelMemBlock* _memBlock, const char* _memBlockName);
size_t disassembleGdelInstruction(gdelMemBlock* _memBlock, size_t _byteCodeArrIndex, fort::utf8_table& _byteCodeTable, fort::utf8_table& _datPoolTable);

#endif // __DEBUG_H__