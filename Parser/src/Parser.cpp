#include "Parser/include/Defines.h"
#include "Parser/include/Parser.h"

Parser::Parser() {
    this->tokenizer = new Tokenizer();
    this->tokenizer->parser = this;
}

Parser::~Parser() {
    delete this->tokenizer;
}

json Parser::parse(const char* _code) {
    this->tokenizer->init(_code);
    this->lookAhead = this->tokenizer->getNextToken();
    return start(_code);
}

json Parser::start(const char* _data) {
    json _root {
        {"type", _PROGRAM},
        {"body", this->statements()["body"].get<json>()}
    };
    
    return _root;
}

json Parser::statements() {
    std::vector<json> _tokens;
    std::string _currentType;
    
    while(strcmp((_currentType = this->lookAhead["type"].get<std::string>()).c_str(), _EOF) != 0) {
        _tokens.push_back(this->tokenizer->get(this));
    }

    _tokens.push_back(this->lookAhead);

    return json {
        {"body", _tokens}
    };
}

json Parser::eatToken(const char* _tokenType) {
    const json _currentToken = this->lookAhead;
    
    if(strcmp(_currentToken["type"].get<std::string>().c_str(), _ERROR) == 0)
        throw UnexpectedEOFException(this->tokenizer->linePointer);
    
    auto _currentTokenType = _currentToken["type"].get<std::string>();
    if(strcmp(_currentTokenType.c_str(), _tokenType) != 0)
        throw UnexpectedTypexception(_tokenType, _currentTokenType.c_str(), this->tokenizer->linePointer);
    
    this->lookAhead = this->tokenizer->getNextToken();
    
    return _currentToken;
}

int Parser::getCurrentParsinLine() const {
    return this->tokenizer->linePointer;
}

const std::string Parser::getCurrentLookAheadType() const{
    return this->lookAhead["type"].get<std::string>();
}

json& Parser::getLookAhead(){
    return this->lookAhead;
}

bool Parser::isEof() const {
    return this->tokenizer->charPointer >= this->tokenizer->codeLenth;
}
