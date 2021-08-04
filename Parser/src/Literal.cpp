#include "Parser/include/Literal.h"
#include "Parser/include/Parser.h"
#include "Parser/include/Exception.hpp"

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
