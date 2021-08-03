#include "json.hpp"
using namespace nlohmann;

#define _INTEGER "INTEGER"
#define _FLOAT "FLOAT"
#define _STRING "STRING"
#define _BOOL "BOOL"

#define _SEMICOLON ";"
#define _SPACE " "
#define _N "/n"
#define _R "/r"
#define _T "/t"
#define _LEFT_COLLIBRACE "{"
#define _RIGHT_COLLIBRACE "}"

#define _IGNORE "IGNORE"
#define _ERROR "ERROR"
#define _EOF "EOF"
#define _PROGRAM "PROGRAM"
#define _EXPRESSION_STATEMENT "EXPRESSION_STATEMENT"
#define _BLOCK_STATEMENT "BLOCK_STATEMENT"

typedef json (*TokenizerFunc)(const char*, int&, int); // token, charPointer, linePointer
typedef bool (*MatchCharFunc)(const char*, int); // the second parameter is for the ones that need lookahead > 1, such as bool
