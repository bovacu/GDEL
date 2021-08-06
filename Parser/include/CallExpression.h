#ifndef __CALLEXPRESSION_H__
#define __CALLEXPRESSION_H__

#include "Parser/include/MemberExpression.h"

class CallExpression {
    private:
        MemberExpression memberExpression;
    
    public:
        json getAst(const Statement& _statement, Parser& _parser, json& _tokenToCheck) const;
        const MemberExpression& getMemberExpression() const;

    private:
        const json callee(const Statement& _statement, Parser& _parser, json& _member) const;
        const std::vector<json> getArguments(const Statement& _statement, Parser& _parser, json& _member) const;
        const std::vector<json> getArgumentList(const Statement& _statement, Parser& _parser, json& _member) const;
};

#endif // __CALLEXPRESSION_H__