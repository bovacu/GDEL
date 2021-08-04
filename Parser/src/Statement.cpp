#include "Parser/include/Statement.h"
#include "Parser/include/Parser.h"

json Statement::getAst(Parser* _parser, json& _tokenToCheck) const {
    if(strcmp(_tokenToCheck["type"].get<std::string>().c_str(), _LEFT_COLLIBRACE) == 0) {
        std::vector<json> _optionalBlockStatement;
        _parser->eatToken(_LEFT_COLLIBRACE);

        while(strcmp(_parser->getCurrentLookAheadType().c_str(), _RIGHT_COLLIBRACE) != 0 && !_parser->isEof())
            _optionalBlockStatement.push_back(this->blockStatement.getAst(*this, _parser, _parser->getLookAhead()));
        
        _parser->eatToken(_RIGHT_COLLIBRACE);

        return json { 
            {"type", _BLOCK_STATEMENT },
            {"body", _optionalBlockStatement}
        };
    }

    return this->expressionStatement.getAst(_parser, _tokenToCheck);
}
