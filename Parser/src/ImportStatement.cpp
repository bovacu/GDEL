#include "Parser/include/ImportStatement.h"
#include "Parser/include/Parser.h"

json ImportStatement::getAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const {
    _parser.eatToken(_IMPORT);
    auto _path = _parser.eatToken(_STRING);
    _parser.eatToken(_SEMICOLON);

    return json {
        {"type", _IMPORT_STATEMENT},
        {"path", _path}
    };
}
