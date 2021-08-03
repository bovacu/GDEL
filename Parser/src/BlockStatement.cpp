#include "Parser/include/BlockStatement.h"
#include "Parser/include/Parser.h"
#include "Parser/include/Statement.h"

json BlockStatement::getAst(const Statement& _statement, Parser* _parser, json& _tokenToCheck) const {
    return _statement.getAst(_parser, _tokenToCheck);
}