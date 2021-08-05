#include "Parser/include/Defines.h"
#include "Parser/include/Statement.h"
#include "Parser/include/Parser.h"

json Statement::getAst(Parser* _parser, json& _tokenToCheck) const {
    auto _tokenType = _tokenToCheck["type"].get<std::string>();

    if(strcmp(_tokenType.c_str(), _LEFT_COLLIBRACE) == 0) {
        std::vector<json> _optionalBlockStatement;
        _parser->eatToken(_LEFT_COLLIBRACE);

        while(strcmp(_parser->getCurrentLookAheadType().c_str(), _RIGHT_COLLIBRACE) != 0 && !_parser->isEof())
            _optionalBlockStatement.push_back(this->blockStatement.getAst(*this, _parser, _parser->getLookAhead()));
        
        _parser->eatToken(_RIGHT_COLLIBRACE);

        return json { 
            {"type", _BLOCK_STATEMENT },
            {"body", _optionalBlockStatement}
        };
    } else if(strcmp(_tokenType.c_str(), _VAR) == 0) {
        return this->variableStatement.getAst(this->expressionStatement.getExpression(), _parser, _parser->getLookAhead());
    } else if (strcmp(_tokenType.c_str(), _IF) == 0) {
        return this->ifStatement.getAst(*this, _parser, _parser->getLookAhead());
    }

    return this->expressionStatement.getAst(_parser, _tokenToCheck);
}


const ExpressionStatement& Statement::getExpressionStatement() const {
    return this->expressionStatement;
}

