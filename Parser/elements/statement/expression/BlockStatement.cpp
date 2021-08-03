#include "BlockStatement.h"
#include "Parser/core/Parser.h"
#include "Parser/elements/statement/Statement.h"

json BlockStatement::getAst(const Statement& _statement, Parser* _parser, json& _tokenToCheck) const {
    return _statement.getAst(_parser, _tokenToCheck);
}