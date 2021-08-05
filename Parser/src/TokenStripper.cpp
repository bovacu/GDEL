#include "Parser/include/Defines.h"
#include "Parser/include/TokenStripper.h"

json TokenStripper::getTokenType(const char* _code, int& _charPointer, int _linePointer) {
    json _outToken;
    numberToken(_outToken, _code, _charPointer, _linePointer);
    stringToken(_outToken, _code, _charPointer, _linePointer);
    boolToken(_outToken, _code, _charPointer, _linePointer);
    nullToken(_outToken, _code, _charPointer, _linePointer);
    idToken(_outToken, _code, _charPointer, _linePointer);
    symbolToken(_outToken, _code, _charPointer, _linePointer);

    return _outToken;
}

void TokenStripper::numberToken(json& _outToken, const char* _code, int& _charPointer, int _linePointer) {
    if(!_outToken.empty()) return;

    // int _min = 48, _max = 57, _ascii = (int)_code[_charPointer];
    // if(_ascii < _min || _ascii > _max)
    //     return;
    if(!std::isdigit(_code[_charPointer])) return;
    
    std::string _fullNumber;
    int _dots = 0;
    while(std::isdigit(_code[_charPointer]) || _code[_charPointer] == '.') {
        if(_code[_charPointer] == '.')
            _dots++;

        if(_dots > 1) break;

        _fullNumber.push_back(_code[_charPointer]);
        _charPointer++;
    }

    if(_dots > 3)
        throw MalformedFloat(_fullNumber.c_str(), _linePointer);

    if(_dots > 1) {
        _charPointer--;
        _fullNumber.erase(std::remove(_fullNumber.begin(), _fullNumber.end(), '.'), _fullNumber.end());
        _outToken = json {
            {"type", _INTEGER},
            {"value",  _fullNumber.c_str()}  
        };
    } else if(_dots == 1) {
        _outToken = json {
            {"type", _FLOAT},
            {"value",  _fullNumber.c_str()}  
        };
    } else {
        _outToken = json {
            {"type", _INTEGER},
            {"value",  _fullNumber.c_str()}  
        };
    }
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
    if(_first != 't' && _first != 'f') return;
    if(_first == 'f' && _charPointer + 4 >= strlen(_code)) return;
    if(_first == 't' && _charPointer + 5 >= strlen(_code)) return;

    std::string _bool = std::string(&_code[_charPointer], &_code[_charPointer] + 4 + (_first == 't' ? 0 : 1));
    if(std::strcmp(_bool.c_str(), "true") != 0 && strcmp(_bool.c_str(), "false") != 0) return;

    std::string _fullBool;
    int _size = _code[_charPointer] == 't' ? 4 : 5;
    for(auto _i = 0; _i < _size; _i++) 
        _fullBool.push_back(_code[_charPointer++]);

    _outToken = json {
        {"type", _BOOL},
        {"value", _fullBool.c_str()}  
    };
}

void TokenStripper::nullToken(json& _outToken, const char* _code, int& _charPointer, int _linePointer) {
    if(!_outToken.empty()) return;

    char _first = _code[_charPointer];
    if(_first != 'n') return;
    if(_first == 'n' && _charPointer + 4 >= strlen(_code)) return;

    std::string _bool = std::string(&_code[_charPointer], &_code[_charPointer] + 4);
    if(std::strcmp(_bool.c_str(), "null") != 0) return;

    std::string _fullNull;
    int _size = 4;
    for(auto _i = 0; _i < _size; _i++) 
        _fullNull.push_back(_code[_charPointer++]);

    _outToken = json {
        {"type", _NULL},
        {"value", _fullNull.c_str()}  
    };
}

void TokenStripper::idToken(json& _outToken, const char* _code, int& _charPointer, int _linePointer) {
    int _ascii = (int)_code[_charPointer], _underscore = 95;
    
    if(_ascii != _underscore && !std::isalpha(_code[_charPointer])) return;

    char _currentChar = _code[_charPointer];
    std::string _fullId;
    int _codeLength = strlen(_code);
    while((_currentChar == '_' || std::isalpha(_currentChar) || std::isdigit(_currentChar)) && _charPointer < _codeLength) {
        _fullId.push_back(_currentChar);
        _charPointer++;
        _currentChar = _code[_charPointer];
    }

    auto _keyword = checkIfIsKeyword(_fullId);

    _outToken = json {
        {"type", _keyword.empty() ? _ID : _keyword},
        {"value", _fullId.c_str()}  
    };
}

std::string TokenStripper::checkIfIsKeyword(const std::string& _fullId) const {
    for(const char* _keyword : keywords) {
        if(_keyword != nullptr && strcmp(_fullId.c_str(), _keyword) == 0)
            return _keyword;
    }
    return "";
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
        // ! and variants
        case 33: {
                char _nextChar = _code[_charPointer + 1];
                if(_nextChar == '='){
                    _outToken  = json { {"type", _COM_N_EQ}, {"value", "!="}};
                    _charPointer++;
                }        
                else
                    _outToken  = json { {"type", _COM_N}, {"value", "!"}};        
                break;
            }
        
        // Comments
        case 35: {
            auto _length = strlen(_code);
            while(_code[_charPointer] != '\n' && _code[_charPointer] != '\r' && _charPointer < _length)
                _charPointer++;

            _charPointer--;
            _outToken = json { {"type", _IGNORE}, {"value", "#"}};     break;
        }
        case 37: _outToken  = json { {"type", _MOD}, {"value", "%"}};               break;

        // & and variants
        case 38: {
                char _nextChar = _code[_charPointer + 1];
                if(_nextChar == '='){
                    _outToken  = json { {"type", _AEQ}, {"value", "&="}};
                    _charPointer++;
                } else if(_nextChar == '&'){
                    _outToken  = json { {"type", _COM_AND}, {"value", "&&"}};
                    _charPointer++;
                } else
                    _outToken  = json { {"type", _EQ}, {"value", "&"}};        
                break;
            }
        case 40: _outToken  = json { {"type", _LEFT_PARENTHESIS}, {"value", "("}};  break;
        case 41: _outToken  = json { {"type", _RIGHT_PARENTHESIS}, {"value", ")"}}; break;
        
        // * and variants
        case 42: {
                char _nextChar = _code[_charPointer + 1];
                if(_nextChar == '='){
                    _outToken  = json { {"type", _BEQ}, {"value", "*="}};  
                    _charPointer++;
                } else if(_nextChar == '*'){
                    _outToken  = json { {"type", _MULTR}, {"value", "**"}};  
                    _charPointer++;
                } else
                    _outToken  = json { {"type", _MUL}, {"value", "*"}};        
                break;
            }
        
        // + and variants
        case 43: {
                char _nextChar = _code[_charPointer + 1];
                if(_nextChar == '='){
                    _outToken  = json { {"type", _PEQ}, {"value", "+="}};  
                    _charPointer++;
                } else if(_nextChar == '+') {
                    _outToken  = json { {"type", _INCR}, {"value", "++"}};  
                    _charPointer++;
                } else
                    _outToken  = json { {"type", _SUM}, {"value", "+"}};        
                break;
            }
        case 44: _outToken  = json { {"type", _COMA}, {"value", ","}};              break;
        
        // - and variants
        case 45: {
                char _nextChar = _code[_charPointer + 1];
                if(_nextChar == '='){
                    _outToken  = json { {"type", _MEQ}, {"value", "-="}};  
                    _charPointer++;
                } else if(_nextChar == '-') {
                    _outToken  = json { {"type", _DECR}, {"value", "--"}};  
                    _charPointer++;
                } else
                    _outToken  = json { {"type", _SUB}, {"value", "-"}};        
                break;
            }
        
        // .. and ...
        case 46 : {
                char _nextChar = _code[_charPointer + 1];
                char _nextNextChar = _code[_charPointer + 2];
                if(_nextChar == '.'){
                    _outToken  = json { {"type", _INCL_RANGE}, {"value", ".."}};  
                    _charPointer++;
                    if(_nextNextChar == '.') {
                        _outToken  = json { {"type", _EXCL_RANGE}, {"value", "..."}};  
                        _charPointer++;
                    }
                }

                break;
            }
        // / and variants
        case 47: {
                char _nextChar = _code[_charPointer + 1];
                if(_nextChar == '='){
                    _outToken  = json { {"type", _DEQ}, {"value", "/="}};  
                    _charPointer++;
                }
                else if(_nextChar == '/'){
                    _outToken  = json { {"type", _DVDR}, {"value", "//"}}; 
                    _charPointer++;
                } else
                    _outToken  = json { {"type", _DIV}, {"value", "/"}};        
                break;
            }
        case 59: _outToken  = json { {"type", _SEMICOLON}, {"value", ";"}};         break;
        
        // < and variants
        case 60: {
                char _nextChar = _code[_charPointer + 1];
                if(_nextChar == '='){
                    _outToken  = json { {"type", _COM_LT_EQ}, {"value", "<="}};        
                    _charPointer++;
                }
                else
                    _outToken  = json { {"type", _COM_LT}, {"value", "<"}};        
                break;
            }

        // = and variants
        case 61: {
                char _nextChar = _code[_charPointer + 1];
                if(_nextChar == '='){
                    _outToken  = json { {"type", _COM_EQ}, {"value", "=="}};
                    _charPointer++;
                }        
                else
                    _outToken  = json { {"type", _EQ}, {"value", "="}};        
                break;
            }
        
        // > and variants
        case 62: {
                char _nextChar = _code[_charPointer + 1];
                if(_nextChar == '='){
                    _outToken  = json { {"type", _COM_GT_EQ}, {"value", ">="}};    
                    _charPointer++;
                }    
                else
                    _outToken  = json { {"type", _COM_GT}, {"value", ">"}};        
                break;
            }
        case 123: _outToken = json { {"type", _LEFT_COLLIBRACE}, {"value", "{"}};   break;

        // | and variants
        case 124: {
                char _nextChar = _code[_charPointer + 1];
                if(_nextChar == '='){
                    _outToken  = json { {"type", _OEQ}, {"value", "|="}};
                    _charPointer++;
                } else if(_nextChar == '|'){
                    _outToken  = json { {"type", _COM_OR}, {"value", "||"}};
                    _charPointer++;
                } else
                    _outToken  = json { {"type", _EQ}, {"value", "|"}};        
                break;
            }
        case 125: _outToken = json { {"type", _RIGHT_COLLIBRACE}, {"value", "}"}};  break;
        default: {
                _outToken = {};
                std::cerr << "Symbol " << _code[_charPointer] << " with ascii " << (int)_code[_charPointer] << " wasn't handled in symbolToken() inside the switch statement, or maybe it wasn't added to the list inside Declarations.h" << std::endl;
                break;
            }
    }

    _charPointer++;
}