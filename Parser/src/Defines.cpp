#include "Parser/include/Defines.h"

const char* symbols[35] = { _SPACE, _SEMICOLON, _N, _R, _T, _SUM, _DIV, _MUL, 
                            _SUB, _MOD, _POW, _LEFT_PARENTHESIS, _RIGHT_PARENTHESIS, 
                            _EQ, _AEQ, _DEQ, _MEQ, _OEQ, _PEQ, _BEQ, _MODEQ, _POWEQ, 
                            _COMA, _COM_EQ, _COM_GT, _COM_GT_EQ, _COM_LT, _COM_LT_EQ,
                            _COM_N, _COM_N_EQ, _COM_AND, _COM_OR, _DOT, _LEFT_BRACKET, 
                            _RIGHT_BRACKET
                          };

const char* keywords[17] = { _VAR, _FOR, _LOOP, _IF, _ELSE_IF, _ELSE, _SWITCH, _CASE,
                             _IN, _FUNC, _RET, _NEW, _STRUCT, _IMPORT, _SELF, _BASE,
                             _EXTENDS 
                            };