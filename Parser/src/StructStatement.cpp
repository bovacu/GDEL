#include "Parser/include/StructStatement.h"
#include "Parser/include/Parser.h"


json StructStatement::getAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const {
    _parser.eatToken(_STRUCT);
    auto _structName = _parser.eatToken(_ID);
    json _inheritance;

    if(strcmp(_parser.getCurrentLookAheadType().c_str(), _EXTENDS) == 0) {
        _parser.eatToken(_EXTENDS);
        _inheritance = _parser.eatToken(_ID);
    }

    auto _structBody = _statement.getBlockStatement().getAst(_statement, _parser, _parser.getLookAhead());

    return json {
        {"type", _STRUCT_STATEMENT},
        {"name", _structName},
        {"extends", _inheritance},
        {"structBody", _structBody}
    };
}


json StructStatement::getSelfAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const {
    _parser.eatToken(_SELF);
    return json {
        {"type", _SELF_EXPRESSION}
    };
}

json StructStatement::getBaseAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const {
    _parser.eatToken(_BASE);
    return json {
        {"type", _BASE_EXPRESSION}
    };
}

json StructStatement::getNewAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const {
    _parser.eatToken(_NEW);
    auto _member = _statement.getExpressionStatement()
                             .getExpression()
                             .getAssigmentExpression()
                             .getLogicalOrExpression()
                             .getLogicalAndExpression()
                             .getEqualityExpression()
                             .getRelationalExpression()
                             .getAdditiveExpression()
                             .getMultiplicativeExpression()
                             .getRangeExpression()
                             .getUnaryExpression()
                             .getCallExpression()
                             .getMemberExpression();
    
    auto _memberCallee = _member.getAst(_statement, _parser, _parser.getLookAhead());
    _parser.eatToken(_LEFT_PARENTHESIS);
    auto _args = _statement.getFunctionStatement().getParamList(_statement, _parser, _parser.getLookAhead());
    _parser.eatToken(_RIGHT_PARENTHESIS);
    return json {
        {"type", _NEW_EXPRESSION},
        {"callee", _memberCallee},
        {"args", _args}
    }; 
}
