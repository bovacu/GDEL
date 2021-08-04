#ifndef __TOKENSTRIPPER_H__
#define __TOKENSTRIPPER_H__

#include "Parser/include/Declarations.h"
#include "Parser/include/Exception.hpp"

class TokenStripper {
    public:
        json getTokenType(const char* _code, int& _charPointer, int _linePointer);

    private:
        void numberToken(json& _outToken, const char* _code, int& _charPointer, int _linePointer);
        void stringToken(json& _outToken, const char* _code, int& _charPointer, int _linePointer);
        void boolToken(json& _outToken, const char* _code, int& _charPointer, int _linePointer);
        void symbolToken(json& _outToken, const char* _code, int& _charPointer, int _linePointer);
};

#endif // __TOKENSTRIPPER_H__