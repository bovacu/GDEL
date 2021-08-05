#include "Parser/include/VariableStatement.h"
#include "Parser/include/Parser.h"
#include "Parser/include/Defines.h"
#include "Parser/include/Expression.h"
#include "Parser/include/Literal.h"

json VariableStatement::getAst(const Expression& _expression, Parser* _parser, json& _tokenToCheck) const {
    _parser->eatToken(_VAR);
    
    auto _declarations = getDeclarationList(_expression, _parser, _parser->getLookAhead());
    
    _parser->eatToken(_SEMICOLON);

    return json { 
        {"type", _VARIABLE_STATEMENT},
        {"declarations", _declarations}
     };
}


std::vector<json> VariableStatement::getDeclarationList(const Expression& _expression, Parser* _parser, json& _tokenToCheck) const {
    std::vector<json> _declarationList;

    _declarationList.push_back(getVariableDeclaration(_expression, _parser, _parser->getLookAhead()));

    while(strcmp(_parser->getCurrentLookAheadType().c_str(), _COMA) == 0) {
        _parser->eatToken(_COMA);
        _declarationList.push_back(getVariableDeclaration(_expression, _parser, _parser->getLookAhead()));
    }

    return _declarationList;
}


json VariableStatement::getVariableDeclaration(const Expression& _expression, Parser* _parser, json& _tokenToCheck) const {
    auto _literal = _expression.getAssigmentExpression()
                               .getLogicalOrExpression()
                               .getLogicalAndExpression()
                               .getEqualityExpression()
                               .getRelationalExpression()
                               .getAdditiveExpression()
                               .getMultiplicativeExpression()
                               .getRangeExpression()
                               .getUnaryExpression()
                               .getPrimaryExpression()
                               .getLiteral();
    auto _id = _literal.getAst(_parser, _parser->getLookAhead());
    auto _initializer = strcmp(_parser->getCurrentLookAheadType().c_str(), _SEMICOLON) != 0 &&
        strcmp(_parser->getCurrentLookAheadType().c_str(), _COMA) != 0 ? getInitializer(_expression, _parser, _parser->getLookAhead()) : json {  };

    return json {
        {"type", _VARIABLE_DECLARATION},
        {"id", _id},
        {"init", _initializer}
    };
}


json VariableStatement::getInitializer(const Expression& _expression, Parser* _parser, json& _tokenToCheck) const {
    _parser->eatToken(_EQ);
    return _expression.getAssigmentExpression().getAst(_expression, _parser, _parser->getLookAhead());
}

