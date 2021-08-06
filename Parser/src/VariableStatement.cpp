#include "Parser/include/VariableStatement.h"
#include "Parser/include/Parser.h"
#include "Parser/include/Statement.h"
#include "Parser/include/Defines.h"
#include "Parser/include/Expression.h"
#include "Parser/include/Literal.h"

json VariableStatement::getAst(const Statement& _statement, Parser* _parser, json& _tokenToCheck) const {
    _parser->eatToken(_VAR);
    
    auto _declarations = getDeclarationList(_statement, _statement.getExpressionStatement().getExpression(), _parser, _parser->getLookAhead());
    
    _parser->eatToken(_SEMICOLON);

    return json { 
        {"type", _VARIABLE_STATEMENT},
        {"declarations", _declarations}
     };
}


std::vector<json> VariableStatement::getDeclarationList(const Statement& _statement, const Expression& _expression, Parser* _parser, json& _tokenToCheck, bool _isSpecialDecl) const {
    std::vector<json> _declarationList;
    
    _declarationList.push_back(getVariableDeclaration(_statement, _expression, _parser, _parser->getLookAhead(), _isSpecialDecl));

    while(strcmp(_parser->getCurrentLookAheadType().c_str(), _COMA) == 0) {
        _parser->eatToken(_COMA);
        _declarationList.push_back(getVariableDeclaration(_statement, _expression, _parser, _parser->getLookAhead(), _isSpecialDecl));
    }
    
    return _declarationList;
}


json VariableStatement::getVariableDeclaration(const Statement& _statement, const Expression& _expression, Parser* _parser, json& _tokenToCheck, bool _isSpecialDecl) const {
    auto _literal = _expression.getAssigmentExpression()
                               .getLogicalOrExpression()
                               .getLogicalAndExpression()
                               .getEqualityExpression()
                               .getRelationalExpression()
                               .getAdditiveExpression()
                               .getMultiplicativeExpression()
                               .getRangeExpression()
                               .getUnaryExpression()
                               .getCallExpression()
                               .getMemberExpression()
                               .getPrimaryExpression()
                               .getLiteral();
    auto _id = _literal.getAst(_statement, _parser, _parser->getLookAhead());

    json _initializer = {};
    if(!_isSpecialDecl)
        _initializer = strcmp(_parser->getCurrentLookAheadType().c_str(), _SEMICOLON) != 0 &&
            strcmp(_parser->getCurrentLookAheadType().c_str(), _COMA) != 0 ? getInitializer(_statement, _expression, _parser, _parser->getLookAhead()) : json {  };
    else {
        if(strcmp(_parser->getCurrentLookAheadType().c_str(), _EQ) == 0)
            _initializer = getInitializer(_statement, _expression, _parser, _parser->getLookAhead());
    }

    return json {
        {"type", _VARIABLE_DECLARATION},
        {"id", _id},
        {"init", _initializer}
    };
}


json VariableStatement::getInitializer(const Statement& _statement, const Expression& _expression, Parser* _parser, json& _tokenToCheck) const {
    _parser->eatToken(_EQ);
    return _expression.getAssigmentExpression().getAst(_statement, _parser, _parser->getLookAhead());
}

