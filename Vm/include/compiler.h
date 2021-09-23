#ifndef __COMPILER_H__
#define __COMPILER_H__

#include "Vm/include/tokenizer.h"
#include "Vm/include/vm.h"

typedef struct {
    gdelToken current;
    gdelToken previous;
    bool hadError;
    bool panicMode;
} gdelParser;

typedef enum {
    PREC_NONE,
    PREC_ASSIGNMENT,  // =              precedence MIN
    PREC_OR,          // or                   |
    PREC_AND,         // and                  |
    PREC_EQUALITY,    // ==                   |
    PREC_COMPARISON,  // < > <= >=            |
    PREC_TERM,        // + -                  |
    PREC_FACTOR,      // * /                  |
    PREC_UNARY,       // - !                  |
    PREC_CALL,        // . ()                 ▼
    PREC_PRIMARY      //                precedence MAX
} gdelPrecedence;

class gdelCompiler;
typedef void (gdelCompiler::*gdelParseFn)(gdelVm&);
typedef struct {
    gdelParseFn prefix;
    gdelParseFn infix;
    gdelPrecedence precedence;
} gdelParseRule;

class gdelCompiler {
    private:
        gdelTokenizer tokenizer;
        gdelParser parser;
        gdelMemBlock* currentCompilingBlock;
        static gdelParseRule rules[45];

    public:
        bool compile(gdelVm& _vm, const char* _code, gdelMemBlock* _memBlock);

    private:
        bool matchToken(gdelTokenType _tokenType);
        bool checkToken(gdelTokenType _tokenType);
        void advance();
        void declaration(gdelVm& _vm);
        void statement(gdelVm& _vm);
        void variable(gdelVm& _vm);
        void varDeclaration(gdelVm& _vm);
        void defineVar(gdelVm& _vm, byte _varAddress);
        void namedVar(gdelVm& _vm, gdelToken _token);
        void expressionStatement(gdelVm& _vm);
        void expression(gdelVm& _vm);
        void consume(gdelTokenType _tokenType, const char* _errorMessage);
        void errorAtCurrent(const char* _errorMessage);
        void errorAt(gdelToken* _token, const char* _errorMessage);
        void error(const char* _errorMessage);

        void parserPrecedence(gdelVm& _vm, gdelPrecedence _precedence);
        byte parseVariable(gdelVm& _vm, const char* _errorMessage);
        gdelParseRule* getParseRule(gdelTokenType _tokenType);

        void literal(gdelVm& _vm);

        void number(gdelVm& _vm);
        byte makeConstant(gdelVm& _vm, gdelData _data);
        void grouping(gdelVm& _vm);
        void unary(gdelVm& _vm);

        void binary(gdelVm& _vm);

        void str(gdelVm& _vm);

        void emitByte(byte _byte);
        void emitBytes(byte _byte0, byte _byte1);
        void emitConstant(gdelVm& _vm, gdelData _data);
        byte emitIdetifierConstant(gdelVm& _vm, gdelToken* _token);
        void emitReturn();
        void endCompiler();


        void synchronize(gdelVm& _vm);


        void printStatement(gdelVm& _vm);
};

#endif // __COMPILER_H__
