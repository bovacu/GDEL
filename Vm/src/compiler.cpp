#include "Vm/include/compiler.h"
#include "Vm/include/memBlock.h"
#include "Vm/include/debug.h"
#include "Vm/include/tokenizer.h"
#include "Vm/include/register.h"
#include <functional>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

gdelParseRule gdelCompiler::rules[] = {
    [gdelTokenType::LEFT_PAREN]    = { &gdelCompiler::grouping,  NULL,                   PREC_NONE },
    [gdelTokenType::RIGHT_PAREN]   = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::LEFT_BRACE]    = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::RIGHT_BRACE]   = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::COMMA]         = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::DOT]           = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::MINUS]         = { &gdelCompiler::unary,     &gdelCompiler::binary,  PREC_TERM },
    [gdelTokenType::PLUS]          = { NULL,                     &gdelCompiler::binary,  PREC_TERM },
    [gdelTokenType::SEMICOLON]     = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::SLASH]         = { NULL,                     &gdelCompiler::binary,  PREC_FACTOR },
    [gdelTokenType::STAR]          = { NULL,                     &gdelCompiler::binary,  PREC_FACTOR },
    [gdelTokenType::PERCENTAGE]    = { NULL,                     &gdelCompiler::binary,  PREC_FACTOR },
    [gdelTokenType::POW]           = { NULL,                     &gdelCompiler::binary,  PREC_FACTOR },
    [gdelTokenType::BANG]          = { &gdelCompiler::unary,     NULL,                   PREC_NONE },
    [gdelTokenType::BANG_EQUAL]    = { NULL,                     &gdelCompiler::binary,  PREC_EQUALITY },
    [gdelTokenType::EQUAL]         = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::EQUAL_EQUAL]   = { NULL,                     &gdelCompiler::binary,  PREC_EQUALITY },
    [gdelTokenType::GREATER]       = { NULL,                     &gdelCompiler::binary,  PREC_COMPARISON },
    [gdelTokenType::GREATER_EQUAL] = { NULL,                     &gdelCompiler::binary,  PREC_COMPARISON },
    [gdelTokenType::LESS]          = { NULL,                     &gdelCompiler::binary,  PREC_COMPARISON },
    [gdelTokenType::LESS_EQUAL]    = { NULL,                     &gdelCompiler::binary,  PREC_COMPARISON },
    [gdelTokenType::IDENTIFIER]    = { &gdelCompiler::variable,  NULL,                   PREC_NONE },
    [gdelTokenType::STRING]        = { &gdelCompiler::str,       NULL,                   PREC_NONE },
    [gdelTokenType::NUMBER]        = { &gdelCompiler::number,    NULL,                   PREC_NONE },
    [gdelTokenType::AND]           = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::REG]           = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::ELSE]          = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::FALSE]         = { &gdelCompiler::literal,   NULL,                   PREC_NONE },
    [gdelTokenType::FOR]           = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::FUNC]          = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::IF]            = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::NULL_]         = { &gdelCompiler::literal,   NULL,                   PREC_NONE },
    [gdelTokenType::OR]            = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::PRINT]         = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::RET]           = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::BASE]          = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::SELF]          = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::TRUE]          = { &gdelCompiler::literal,   NULL,                   PREC_NONE },
    [gdelTokenType::VAR]           = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::LOOP]          = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::ERROR]         = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::EOF_]          = { NULL,                     NULL,                   PREC_NONE },
};

/*
 * This is how the compilation proccess works:
 *
 * 1 + 3
 *
 * compile:
 *          * advance -> sets previous (gdelToken) to the current and the we advance the current to a new token provided by the
 *                       tokenizer. If error, just return the error and stops compiling.
 *                             ~ previous -> empty token
 *                             ~ current  -> number token (1)
 *
 *          * expression -> this calls parserPrecedence(PREC_ASSIGNMENT)
 *                                     |
 *                                     ???> advance:
 *                                     |        ~ previous -> number token (1)
 *                                     |        ~ current  -> op_sum token (+)
 *                                     |
 *                                     ???> Now we get the parsing rule for number and call it.
 *                                     |                                  |
 *                                     |                                  ???> emitConst(1) (*1)
 *                                     |                                     |
 *                                     |                                     ???> emitBytes(OP_CONST, makeConst(1))
 *                                     |                                        |                   |
 *                                     |                                        |                   ???> Writes the constant to the data pool and returns it's address inside the DP
 *                                     |                                        |
 *                                     |                                        ???> emitByte(OP_CONST) -> this writes the OP_CONST byte to the gdelMemBlock
 *                                     |                                        |
 *                                     |                                        ???> emitByte(1 addrss inside DP) -> this writes the OP_CONST byte to the gdelMemBlock
 *                                     |
 *                                     ???> Now we check if the passed precedence (PREC_ASSIGNMENT) is lower than the precedence of our previous (PREC_TERM, as previous is +).
 *                                          |
 *                                          ???> advance()
 *                                          |        ~ previous -> number token (+)
 *                                          |        ~ current  -> op_sum token (3)
 *                                          |
 *                                          ???> The prefix for the rule + is binary
 *                                                                          |
 *                                                                          ???> We check if there is a more priority op (saved as op) with parserPrecedence(previous.precedence + 1) -> PREC_FACTOR
 *                                                                          |       |
 *                                                                          |       ???> advance
 *                                                                          |       |        ~ previous -> number token (3)
 *                                                                          |       |        ~ current  -> op_sum token (/0)
 *                                                                          |       ???> Now we get the parsing rule for number and call it.
 *                                                                          |       |   ???> emitConst(3) (*2)
 *                                                                          |       |       |
 *                                                                          |       |       ???> emitBytes(OP_CONST, makeConst(3))
 *                                                                          |       |       |                   |
 *                                                                          |       |       |                   ???> Writes the constant to the data pool and returns it's address inside the DP
 *                                                                          |       |       |
 *                                                                          |       |       ???> emitByte(OP_CONST) -> this writes the OP_CONST byte to the gdelMemBlock
 *                                                                          |       |       |
 *                                                                          |       |       ???> emitByte(3 addrss inside DP) -> this writes the OP_CONST byte to the gdelMemBlock
 *                                                                          |       |
 *                                                                          |       ???> Now we check the precedence passed (PREC_TERM) is lower than the previous precedecene (PREC_NON as is number). So no while here
 *                                                                          |
 *                                                                          ???> Now we emit the byte of the corresponding operator (op) and emitByte(OP_ADD) (*3)
 *
 *
 * This ends the compilation process which also prepares the gdelMemBlock for the Vm to execute, and it is finally like this:
 *
 *     ?????????????????????????????????????????????????????????
 *   2 ???   OP_ADD (*3)   ???
 *     ?????????????????????????????????????????????????????????               ?????????????????????????????????????????????????????????
 *   1 ???  OP_CONST (*2)  ??? ????????????????????????????????????> ???        3        ???
 *     ?????????????????????????????????????????????????????????               ?????????????????????????????????????????????????????????
 *   0 ???  OP_CONST (*1)  ??? ????????????????????????????????????> ???        1        ???
 *     ?????????????????????????????????????????????????????????               ?????????????????????????????????????????????????????????
 *
 * And now the Vm will just simply execute what the gdelMemblock has inside
*/

bool gdelCompiler::compile(gdelVm& _vm, const char* _code, gdelMemBlock* _memBlock) {
    this->tokenizer.init(_code);
    this->currentCompilingBlock = _memBlock;
    this->parser.panicMode = false;
    this->parser.hadError = false;

    advance();
    while(!matchToken(gdelTokenType::EOF_)) {
        declaration(_vm);
    }
    consume(gdelTokenType::EOF_, "Expect end of expression.");

    endCompiler();
    return !this->parser.hadError;
}

void gdelCompiler::declaration(gdelVm& _vm) {
    if(matchToken(gdelTokenType::VAR))
        varDeclaration(_vm);
    else
        statement(_vm);
    
    if(this->parser.panicMode) synchronize(_vm);
}

void gdelCompiler::statement(gdelVm& _vm) {
    if(matchToken(gdelTokenType::PRINT))
        printStatement(_vm);
    else
        expressionStatement(_vm);
}

void gdelCompiler::variable(gdelVm& _vm) {
    namedVar(_vm, this->parser.previous);
}

void gdelCompiler::varDeclaration(gdelVm& _vm) {
    uint8_t _global = parseVariable(_vm, "Expect variable name.");

    if (matchToken(gdelTokenType::EQUAL))
        expression(_vm);
    else 
        emitByte(gdelOpCode::OP_NULL);
    
    // consume(TOKEN_SEMICOLON, "Expect ';' after variable declaration.");

    defineVar(_vm, _global);
}

void gdelCompiler::defineVar(gdelVm& _vm, byte _varAddress) {
    emitBytes(gdelOpCode::OP_DEFINE_GLOBAL_VAR, _varAddress);
}

void gdelCompiler::namedVar(gdelVm& _vm, gdelToken _token) {
    byte _arg = emitIdetifierConstant(_vm, &_token);
    emitBytes(gdelOpCode::OP_GET_GLOBAL_VAR, _arg);
}

void gdelCompiler::expressionStatement(gdelVm& _vm) {
    expression(_vm);
    emitByte(gdelOpCode::OP_POP);
}

void gdelCompiler::expression(gdelVm& _vm) {
    parserPrecedence(_vm, gdelPrecedence::PREC_ASSIGNMENT);
}

bool gdelCompiler::matchToken(gdelTokenType _tokenType) {
    if(!checkToken(_tokenType)) return false;
    advance();
    return true;
}

bool gdelCompiler::checkToken(gdelTokenType _tokenType) {
    return parser.current.type == _tokenType;
}

void gdelCompiler::advance() {
    this->parser.previous = this->parser.current;
    for (;;) {
      this->parser.current = this->tokenizer.getToken();
      if (this->parser.current.type != gdelTokenType::ERROR) break;
      errorAtCurrent(this->parser.current.start);
    }
}


void gdelCompiler::consume(gdelTokenType _tokenType, const char* _errorMessage) {
    if (this->parser.current.type == _tokenType) {
        advance();
        return;
    }

    errorAtCurrent(_errorMessage);
}

void gdelCompiler::errorAtCurrent(const char* _errorMessage) {
    errorAt(&this->parser.current, _errorMessage);
}

void gdelCompiler::errorAt(gdelToken* token, const char* message) {
    if(this->parser.panicMode) return;
    this->parser.panicMode = true;

    PRINT_ERROR("[line " << token->line << "] Error");
    if (token->type == gdelTokenType::EOF_) {
        PRINT_ERROR(" at end");
    } else if (token->type == gdelTokenType::ERROR) {
        PRINT_ERROR(" fuck");
    } else {
        std::string _err(token->start, token->start + token->length);
        PRINT_ERROR(" at '" << _err << "'");
        // fprintf(stderr, " at '%.*s'", token->length, token->start);
    }
    PRINT_LN_ERROR(": " << message);
    this->parser.hadError = true;
}

void gdelCompiler::error(const char* _errorMessage) {
    errorAt(&this->parser.previous, _errorMessage);
}

/*
 * This is the way of controlling until what point an expression should be parsed. This method works with the enum defined
 * in the gdelPrecedence inside compiler.h, there it is stablished the precedence.
 *
 * If a precedence of X is being parsed, all the upper precedence elements will be parsed before the currente precedence, and
 * the lower precedence elements will never be executed. So if we have the expression -fibonacci(10); we have an Unary
 * precedence, a Call precedence and a Primary precedence. The precedence values, from higher to lower is
 * Primary -> Call -> Unary, so first of all the parser will evaluate 4, then it will call fibonacci and finally it will negate
 * the result, in that exact order and won't take any lower precedence operations than Unary.
 *
 * -fibonacci(4) -- Primary: 4                  -> calls the rule to handle a number
 *                     |
 *                     ???
 *                  Call: fibonacci(Primary)    -> calls the rule to handle a call
 *                     |
 *                     ???
 *                  Unary: -Call(Primary)       -> calls the rule to handle a unary
 *
*/
void gdelCompiler::parserPrecedence(gdelVm& _vm, gdelPrecedence _precedence) {
    advance();

    gdelParseRule* _rule = getParseRule(this->parser.previous.type);
    if (_rule->prefix == nullptr) {
        error("Expected expression");
        return;
    }
    gdelParseFn _prefix = _rule->prefix;
    ((*this).*_prefix)(_vm); // As this is a function pointer-to-member, we always need an object to invoke the function, in this case is 'this'

    while (_precedence <= getParseRule(this->parser.current.type)->precedence) {
        advance();
        _rule = getParseRule(this->parser.previous.type);
        gdelParseFn _infix = _rule->infix;
        ((*this).*_infix)(_vm); // Same as above
    }
}

byte gdelCompiler::parseVariable(gdelVm& _vm, const char* _errorMessage) {
    consume(gdelTokenType::IDENTIFIER, _errorMessage);
    return emitIdetifierConstant(_vm, &this->parser.previous);
}

gdelParseRule* gdelCompiler::getParseRule(gdelTokenType _tokenType) {
    return &rules[_tokenType];
}

/*
 * This function returns types of values that don't have many variants. Currently we have
 *      - bool
 *      - null
 *
 * But any new type can be added here, for example EMPTY for lists and dictionaries
*/
void gdelCompiler::literal(gdelVm& _vm) {
    switch (this->parser.previous.type) {
      case gdelTokenType::FALSE: emitByte(gdelOpCode::OP_FALSE); break;
      case gdelTokenType::NULL_: emitByte(gdelOpCode::OP_NULL); break;
      case gdelTokenType::TRUE: emitByte(gdelOpCode::OP_TRUE); break;
      default:
        return; // Unreachable.
    }
}

void gdelCompiler::number(gdelVm& _vm) {
    auto _constant = strtod(this->parser.previous.start, nullptr);
    emitConstant(_vm, CREATE_GDEL_NUMBER(_constant));
}


byte gdelCompiler::makeConstant(gdelVm& _vm, gdelData _data) {
    int constant = writeConstantToGdelMemBlock(this->currentCompilingBlock, _data);
    if (constant > UINT8_MAX) {
        error("Too many constants in one chunk.");
        return 0;
    }
    return (uint8_t)constant;
}

void gdelCompiler::grouping(gdelVm& _vm) {
    expression(_vm);
    consume(gdelTokenType::RIGHT_PAREN, "Expect ')' after expression.");
}

void gdelCompiler::unary(gdelVm& _vm) {
    gdelTokenType operatorType = this->parser.previous.type;
    parserPrecedence(_vm, gdelPrecedence::PREC_UNARY);

    switch (operatorType) {
        case gdelTokenType::MINUS: emitByte(gdelOpCode::OP_NEGATE); break;
        case gdelTokenType::BANG:  emitByte(gdelOpCode::OP_NOT); break;
        default:
            return; // Unreachable.
    }
}

void gdelCompiler::binary(gdelVm& _vm) {
    gdelTokenType operatorType = this->parser.previous.type;
    gdelParseRule* rule = getParseRule(operatorType);
    parserPrecedence(_vm, (gdelPrecedence)(rule->precedence + 1));
    
    switch (operatorType) {
        case gdelTokenType::PLUS:          emitByte(gdelOpCode::OP_ADD);                            break;
        case gdelTokenType::MINUS:         emitByte(gdelOpCode::OP_SUB);                            break;
        case gdelTokenType::STAR:          emitByte(gdelOpCode::OP_MUL);                            break;
        case gdelTokenType::SLASH:         emitByte(gdelOpCode::OP_DIV);                            break;
        case gdelTokenType::PERCENTAGE:    emitByte(gdelOpCode::OP_PERCENT);                        break;
        case gdelTokenType::POW:           emitByte(gdelOpCode::OP_POW);                            break;
        case gdelTokenType::BANG_EQUAL:    emitBytes(gdelOpCode::OP_EQUAL, gdelOpCode::OP_NOT);     break;
        case gdelTokenType::EQUAL_EQUAL:   emitByte(gdelOpCode::OP_EQUAL);                          break;
        case gdelTokenType::GREATER:       emitByte(gdelOpCode::OP_GREAT);                          break;
        case gdelTokenType::GREATER_EQUAL: emitBytes(gdelOpCode::OP_LESS, gdelOpCode::OP_NOT);      break;
        case gdelTokenType::LESS:          emitByte(gdelOpCode::OP_LESS);                           break;
        case gdelTokenType::LESS_EQUAL:    emitBytes(gdelOpCode::OP_GREAT, gdelOpCode::OP_NOT);     break;
        default: return; // Unreachable.
    }
}

/*
 * +1 is to trim the first ' and - 2 is to trim the last ' 
*/
void gdelCompiler::str(gdelVm& _vm) {
    auto _reg = copyString(_vm, this->parser.previous.start + 1, this->parser.previous.length - 2);
    
    emitConstant(_vm, CREATE_GDEL_REGISTER(_reg));
}

void gdelCompiler::emitBytes(byte _byte0, byte _byte1) {
    emitByte(_byte0);
    emitByte(_byte1);
}

void gdelCompiler::emitByte(byte _byte) {
    writeToGdelMemBlock(this->currentCompilingBlock, _byte);
}

void gdelCompiler::emitConstant(gdelVm& _vm, gdelData _data) {
    emitBytes(gdelOpCode::OP_CONST, makeConstant(_vm, _data));
}

byte gdelCompiler::emitIdetifierConstant(gdelVm& _vm, gdelToken* _token) {
    return makeConstant(_vm, CREATE_GDEL_REGISTER(copyString(_vm, _token->start, _token->length)));
}

void gdelCompiler::emitReturn() {
    emitByte(gdelOpCode::OP_RETURN);
}

void gdelCompiler::endCompiler() {
    emitReturn();
}


void gdelCompiler::synchronize(gdelVm& _vm) {
    this->parser.panicMode = false;

    while (this->parser.current.type != gdelTokenType::EOF_) {
        // if (this->parser.previous.type == gdelTokenType::SEMICOLON) return;
        switch (this->parser.current.type) {
        case gdelTokenType::REG:
        case gdelTokenType::FUNC:
        case gdelTokenType::VAR:
        case gdelTokenType::FOR:
        case gdelTokenType::IF:
        case gdelTokenType::LOOP:
        case gdelTokenType::PRINT:
        case gdelTokenType::RET:
            return;

        default:
            ; // Do nothing.
        }

        advance();
    }
}



void gdelCompiler::printStatement(gdelVm& _vm) {
    expression(_vm);
    // consume(gdelTokenType::SEMICOLON, "Expected ';' at the end of the expression");
    emitByte(gdelOpCode::OP_PRINT);
}