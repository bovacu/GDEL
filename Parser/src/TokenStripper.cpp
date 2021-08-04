#include "Parser/include/TokenStripper.h"

json TokenStripper::getTokenType(const char* _code, int& _charPointer, int _linePointer) {
    json _outToken;
    numberToken(_outToken, _code, _charPointer, _linePointer);
    stringToken(_outToken, _code, _charPointer, _linePointer);
    boolToken(_outToken, _code, _charPointer, _linePointer);
    symbolToken(_outToken, _code, _charPointer, _linePointer);

    return _outToken;
}

void TokenStripper::numberToken(json& _outToken, const char* _code, int& _charPointer, int _linePointer) {
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

void TokenStripper::stringToken(json& _outToken, const char* _code, int& _charPointer, int _linePointer) {
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

void TokenStripper::boolToken(json& _outToken, const char* _code, int& _charPointer, int _linePointer) {
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

void TokenStripper::symbolToken(json& _outToken, const char* _code, int& _charPointer, int _linePointer) {
    if(!_outToken.empty()) return;
    //      /r    to  /t            space to /                :  to  @               [   to   _               {   to    ~
    int _min0 = 9, _max0 = 13, _min1 = 32, _max1 = 47, _min2 = 58, _max2 = 64, _min3 = 91, _max3 = 95, _min4 = 123, _max4 = 126, _ascii = (int)_code[_charPointer];
    if((_ascii < _min0 || _ascii > _max0) && (_ascii < _min1 || _ascii > _max1) && (_ascii < _min2 || _ascii > _max2) && 
       (_ascii < _min3 || _ascii > _max3) && (_ascii < _min4 || _ascii > _max4)) return;

    switch (_ascii) {
        case 9:  _outToken  = json { {"type", _IGNORE}, {"value", "/r"}};           break;
        case 10: _outToken  = json { {"type", _IGNORE}, {"value", "/n"}};           break;
        case 13: _outToken  = json { {"type", _IGNORE}, {"value", "/t"}};           break;
        case 32: _outToken  = json { {"type", _IGNORE}, {"value", " "}};            break;
        case 35: {
            auto _length = strlen(_code);
            while(_code[_charPointer] != '\n' && _code[_charPointer] != '\r' && _charPointer < _length)
                _charPointer++;

            _charPointer--;
            _outToken = json { {"type", _IGNORE}, {"value", "#"}};     break;
        }
        case 37: _outToken  = json { {"type", _MOD}, {"value", "%"}};               break;
        case 40: _outToken  = json { {"type", _LEFT_PARENTHESIS}, {"value", "("}};  break;
        case 41: _outToken  = json { {"type", _RIGHT_PARENTHESIS}, {"value", ")"}}; break;
        case 42: _outToken  = json { {"type", _MUL}, {"value", "*"}};               break;
        case 43: _outToken  = json { {"type", _SUM}, {"value", "+"}};               break;
        case 45: _outToken  = json { {"type", _SUB}, {"value", "-"}};               break;
        case 47: _outToken  = json { {"type", _DIV}, {"value", "/"}};               break;
        case 59: _outToken  = json { {"type", _SEMICOLON}, {"value", ";"}};         break;
        case 123: _outToken = json { {"type", _LEFT_COLLIBRACE}, {"value", "{"}};   break;
        case 125: _outToken = json { {"type", _RIGHT_COLLIBRACE}, {"value", "}"}};  break;
        default: {
                _outToken = {};
                std::cerr << "Symbol " << _code[_charPointer] << " with ascii " << (int)_code[_charPointer] << " wasn't handled in symbolToken() inside the switch statement, or maybe it wasn't added to the list inside Literal.h" << std::endl;
                break;
            }
    }

    _charPointer++;
}