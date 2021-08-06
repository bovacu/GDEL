#include "Parser/include/BlockStatement.h"
#include "Parser/include/Parser.h"

json BlockStatement::getAst(const Statement& _statement, Parser* _parser, json& _tokenToCheck) const {
    std::vector<json> _optionalBlockStatement;
    _parser->eatToken(_LEFT_COLLIBRACE);

    while(strcmp(_parser->getCurrentLookAheadType().c_str(), _RIGHT_COLLIBRACE) != 0 && !_parser->isEof())
        _optionalBlockStatement.push_back(_statement.getAst(_parser, _parser->getLookAhead()));
    
    _parser->eatToken(_RIGHT_COLLIBRACE);

    return json { 
        {"type", _BLOCK_STATEMENT },
        {"body", _optionalBlockStatement}
    };
}