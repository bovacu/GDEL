#include "Parser/include/Literal.h"
#include "Parser/include/Parser.h"
#include "Parser/include/Exception.hpp"

json Literal::getNextToken(Parser* _parser, const char* _code, int& _charPointer, int _linePointer) {
    json _outToken;

    numberToken(_parser, _outToken, _code, _charPointer, _linePointer);
    stringToken(_parser, _outToken, _code, _charPointer, _linePointer);
    boolToken(_parser, _outToken, _code, _charPointer, _linePointer);
    symbolsToken(_parser, _outToken, _code, _charPointer, _linePointer);

    return _outToken;
}

void Literal::numberToken(Parser* _parser, json& _outToken, const char* _code, int& _charPointer, int _linePointer) {
    if(!_outToken.empty()) return;

    int _min = 48, _max = 57, _ascii = (int)_code[_charPointer];
    if(_ascii < _min || _ascii > _max)
        return;
    
    std::string _fullNumber;
    int _isFloat = 0;
    while(std::isdigit(_code[_charPointer]) || _code[_charPointer] == '.') {
        if(_code[_charPointer] == '.')
            _isFloat++;

        if(_isFloat > 1)
            throw MalformedFloat(_fullNumber.c_str(), _linePointer);

        _fullNumber.push_back(_code[_charPointer]);
        _charPointer++;
    }

    _outToken = json {
        {"type", _isFloat == 0 ? _INTEGER : _FLOAT},
        {"value", _fullNumber.c_str()}  
    };
}

void Literal::stringToken(Parser* _parser, json& _outToken, const char* _code, int& _charPointer, int _linePointer) {
    if(!_outToken.empty()) return;

    int _quote = 39, _ascii = (int)_code[_charPointer];
    if(_ascii != _quote) return;

    std::string _fullString;
    int _codeLength = strlen(_code);
    do {
        _fullString.push_back(_code[_charPointer++]);
    } while ((int)_code[_charPointer] != 39 && _charPointer != _codeLength);

    // Let's check that the string is closed correctly
    if(_charPointer <= _codeLength && (int)_code[_charPointer] != 39)
        throw MalformedStringException(_fullString.c_str(), _linePointer);

    _charPointer++;

    _outToken = json {
        {"type", _STRING},
        {"value", _fullString.substr(1).c_str()}  
    };
}

void Literal::boolToken(Parser* _parser, json& _outToken, const char* _code, int& _charPointer, int _linePointer) {
    if(!_outToken.empty()) return;

    char _first = _code[_charPointer];
    if(_charPointer + 4 + (_first == 't' ? 0 : 1) >= strlen(_code) || _first != 't' && _first != 'f') return;
    std::string _bool = std::string(&_code[_charPointer], &_code[_charPointer] + 4 + (_first == 't' ? 0 : 1));
    if(std::strcmp(_bool.c_str(), "true") == 1 && strcmp(_bool.c_str(), "false") == 1) return;

    std::string _fullBool;
    int _size = _code[_charPointer] == 't' ? 4 : 5;
    for(auto _i = 0; _i < _size; _i++) 
        _fullBool.push_back(_code[_charPointer++]);

    _outToken = json {
        {"type", _BOOL},
        {"value", _fullBool.c_str()}  
    };
}

void Literal::symbolsToken(Parser* _parser, json& _outToken, const char* _code, int& _charPointer, int _line) {
    if(!_outToken.empty()) return;
    //      /r    to  /t            space to /                :  to  @               [   to   _               {   to    ~
    int _min0 = 9, _max0 = 13, _min1 = 32, _max1 = 47, _min2 = 58, _max2 = 64, _min3 = 91, _max3 = 95, _min4 = 123, _max4 = 126, _ascii = (int)_code[_charPointer];
    if((_ascii < _min0 || _ascii > _max0) && (_ascii < _min1 || _ascii > _max1) && (_ascii < _min2 || _ascii > _max2) && 
       (_ascii < _min3 || _ascii > _max3) && (_ascii < _min4 || _ascii > _max4)) return;

    switch (_ascii) {
        case 9:  _outToken = json { {"type", _IGNORE}, {"value", "/r"}};    break;
        case 10: _outToken = json { {"type", _IGNORE}, {"value", "/n"}};    break;
        case 13: _outToken = json { {"type", _IGNORE}, {"value", "/t"}};    break;
        case 32: _outToken = json { {"type", _IGNORE}, {"value", " "}};     break;
        case 35: {
            auto _length = strlen(_code);
            while(_code[_charPointer] != '\n' && _code[_charPointer] != '\r' && _charPointer < _length)
                _charPointer++;

            _charPointer--;
            _outToken = json { {"type", _IGNORE}, {"value", "#"}};     break;
        }
        case 59: _outToken  = json { {"type", _SEMICOLON}, {"value", ";"}};  break;
        case 123: _outToken = json { {"type", _LEFT_COLLIBRACE}, {"value", "{"}};  break;
        case 125: _outToken = json { {"type", _RIGHT_COLLIBRACE}, {"value", "}"}};  break;
        default:
            break;
    }

    _charPointer++;
}

// ------------------------- AST

json Literal::getAst(Parser* _parser, json& _tokenToCheck) const {
    auto _tokenType = _tokenToCheck["type"].get<std::string>();
    json _outToken {};
    if(strcmp(_tokenType.c_str(), _INTEGER) == 0 || strcmp(_tokenType.c_str(), _FLOAT) == 0) {
        _outToken = numberAst(_parser, _tokenToCheck);
    } else if(strcmp(_tokenType.c_str(), _STRING) == 0) {
        _outToken = stringAst(_parser, _tokenToCheck);
    } else if(strcmp(_tokenType.c_str(), _BOOL) == 0) {
        _outToken = boolAst(_parser, _tokenToCheck);
    } else {
        for(const char* _symbol : this->symbols)
            if(strcmp(_symbol, _tokenType.c_str()) == 0)
                _outToken = symbolAst(_parser, _tokenToCheck);
    }

    return _outToken;
}

json Literal::numberAst(Parser* _parser, json& _tokenToCheck) const {
    auto _tokenValue = _tokenToCheck["value"].get<std::string>();
    auto _type = _tokenValue.find('.') != std::string::npos ? _FLOAT : _INTEGER;
    auto _token = _parser->eatToken(_type);
    return _token;
}

json Literal::stringAst(Parser* _parser, json& _tokenToCheck) const {
    return _parser->eatToken(_STRING);
}

json Literal::boolAst(Parser* _parser, json& _tokenToCheck) const {
    return _parser->eatToken(_BOOL);
}

json Literal::symbolAst(Parser* _parser, json& _tokenToCheck) const {
    auto _tokenType = _tokenToCheck["type"].get<std::string>();
    for(const char* _symbol : this->symbols)
            if(strcmp(_symbol, _tokenType.c_str()) == 0)
                return _parser->eatToken(_tokenType.c_str());
    throw UnexpectedTokenException(_tokenToCheck["value"].get<std::string>().c_str()[0], _parser->getCurrentParsinLine());
}
