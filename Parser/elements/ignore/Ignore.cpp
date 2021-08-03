#include "Ignore.h"
#include "Parser/core/Parser.h"
#include "Parser/core/Exception.hpp"

void Ignore::assignMethods(std::map<MatchCharFunc, TokenizerFunc>& _map) {
    // WHITESPACES, /r, /n, HT
    std::pair<MatchCharFunc, TokenizerFunc> _whiteSpaces;
    _whiteSpaces.first = [](const char* _token, int _charPointer) -> bool {
        int _whiteSpace = 32, _r = 10, _n = 13, _ht = 9, _ascii = (int)_token[_charPointer];
        return _ascii == _whiteSpace || _ascii == _r || _ascii == _n || _ascii == _ht;
    };
    _whiteSpaces.second = [](const char* _token, int& _charPointer, int _linePoiner) -> json {
        std::string _value = (int)_token[_charPointer] == 13 || (int)_token[_charPointer] == 10 ? "EOL" : "";
        _charPointer++;
        return json {
            {"type", _IGNORE},
            {"value", _value}  
        };
    };
    _map.emplace(_whiteSpaces);

    // COMMENTS (#)
    std::pair<MatchCharFunc, TokenizerFunc> _comments;
    _comments.first = [](const char* _token, int _charPointer) -> bool {
        int _hashtag = 35, _ascii = (int)_token[_charPointer];
        return _ascii == _hashtag;
    };
    _comments.second = [](const char* _token, int& _charPointer, int _linePoiner) -> json {
        int _codeLength = strlen(_token);
        while((int)_token[_charPointer] != 10 && (int)_token[_charPointer] != 13 && _charPointer != _codeLength) {
            _charPointer++;
        }

        return json {
            {"type", _IGNORE},
            {"value", ""}  
        };
    };
    _map.emplace(_comments);
}