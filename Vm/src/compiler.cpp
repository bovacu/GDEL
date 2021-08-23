#include "Vm/include/compiler.h"
#include "Vm/include/memBlock.h"
#include "Vm/include/debug.h"
#include "Vm/include/tokenizer.h"
#include <functional>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <optional>

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
    [gdelTokenType::BANG]          = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::BANG_EQUAL]    = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::EQUAL]         = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::EQUAL_EQUAL]   = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::GREATER]       = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::GREATER_EQUAL] = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::LESS]          = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::LESS_EQUAL]    = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::IDENTIFIER]    = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::STRING]        = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::NUMBER]        = { &gdelCompiler::number,    NULL,                   PREC_NONE },
    [gdelTokenType::AND]           = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::REG]           = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::ELSE]          = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::FALSE]         = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::FOR]           = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::FUNC]          = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::IF]            = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::NULL_]         = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::OR]            = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::PRINT]         = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::RET]           = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::BASE]          = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::SELF]          = { NULL,                     NULL,                   PREC_NONE },
    [gdelTokenType::TRUE]          = { NULL,                     NULL,                   PREC_NONE },
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
 *                                     └> advance: 
 *                                     |        ~ previous -> number token (1)
 *                                     |        ~ current  -> op_sum token (+)
 *                                     |
 *                                     └> Now we get the parsing rule for number and call it.
 *                                     |                                  |
 *                                     |                                  └> emitConst(1) (*1)
 *                                     |                                     |
 *                                     |                                     └> emitBytes(OP_CONST, makeConst(1))
 *                                     |                                        |                   |
 *                                     |                                        |                   └> Writes the constant to the data pool and returns it's address inside the DP
 *                                     |                                        |    
 *                                     |                                        └> emitByte(OP_CONST) -> this writes the OP_CONST byte to the gdelMemBlock
 *                                     |                                        |    
 *                                     |                                        └> emitByte(1 addrss inside DP) -> this writes the OP_CONST byte to the gdelMemBlock
 *                                     |
 *                                     └> Now we check if the passed precedence (PREC_ASSIGNMENT) is lower than the precedence of our previous (PREC_TERM, as previous is +).
 *                                          |
 *                                          └> advance()      
 *                                          |        ~ previous -> number token (+)
 *                                          |        ~ current  -> op_sum token (3)
 *                                          |
 *                                          └> The prefix for the rule + is binary
 *                                                                          |
 *                                                                          └> We check if there is a more priority op (saved as op) with parserPrecedence(previous.precedence + 1) -> PREC_FACTOR
 *                                                                          |       |
 *                                                                          |       └> advance
 *                                                                          |       |        ~ previous -> number token (3)
 *                                                                          |       |        ~ current  -> op_sum token (/0)
 *                                                                          |       └> Now we get the parsing rule for number and call it.
 *                                                                          |       |   └> emitConst(3) (*2)
 *                                                                          |       |       |
 *                                                                          |       |       └> emitBytes(OP_CONST, makeConst(3))
 *                                                                          |       |       |                   |
 *                                                                          |       |       |                   └> Writes the constant to the data pool and returns it's address inside the DP
 *                                                                          |       |       |    
 *                                                                          |       |       └> emitByte(OP_CONST) -> this writes the OP_CONST byte to the gdelMemBlock
 *                                                                          |       |       |    
 *                                                                          |       |       └> emitByte(3 addrss inside DP) -> this writes the OP_CONST byte to the gdelMemBlock          
 *                                                                          |       |
 *                                                                          |       └> Now we check the precedence passed (PREC_TERM) is lower than the previous precedecene (PREC_NON as is number). So no while here   
 *                                                                          |
 *                                                                          └> Now we emit the byte of the corresponding operator (op) and emitByte(OP_ADD) (*3)
 * 
 * 
 * This ends the compilation process which also prepares the gdelMemBlock for the Vm to execute, and it is finally like this:
 * 
 *     ╔═════════════════╗                
 *     ║   OP_ADD (*3)   ║                
 *     ╠═════════════════╣               ╔═════════════════╗
 *     ║  OP_CONST (*2)  ║ ────────────> ║        3        ║
 *     ╠═════════════════╣               ╠═════════════════╣
 *     ║  OP_CONST (*1)  ║ ────────────> ║        1        ║ 
 *     ╚═════════════════╝               ╚═════════════════╝
 * 
 * And now the Vm will just simply execute what the gdelMemblock has inside
*/                                                                          

bool gdelCompiler::compile(gdelVm& _vm, const char* _code, gdelMemBlock* _memBlock) {
    this->tokenizer.init(_code);
    this->currentCompilingBlock = _memBlock;
    this->parser.panicMode = false;
    this->parser.hadError = false;
    
    advance();
    expression(_vm);
    consume(gdelTokenType::EOF_, "Expect end of expression.");

    endCompiler();
    return !this->parser.hadError;
}

void gdelCompiler::advance() {
    this->parser.previous = this->parser.current;
    for (;;) {
      this->parser.current = this->tokenizer.getToken();
      if (this->parser.current.type != gdelTokenType::ERROR) break;
      errorAtCurrent(this->parser.current.start);
    }
}

void gdelCompiler::expression(gdelVm& _vm) {
    parserPrecedence(_vm, gdelPrecedence::PREC_ASSIGNMENT);
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
 *                     ▼
 *                  Call: fibonacci(Primary)    -> calls the rule to handle a call
 *                     |
 *                     ▼
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
        std::cout << "precedence tested: " << _precedence << " against " << getParseRule(this->parser.current.type)->precedence << std::endl;
        advance();
        _rule = getParseRule(this->parser.previous.type);
        gdelParseFn _infix = _rule->infix;
        ((*this).*_infix)(_vm); // Same as above
    }
}

gdelParseRule* gdelCompiler::getParseRule(gdelTokenType _tokenType) {
    return &rules[_tokenType];
}

void gdelCompiler::number(gdelVm& _vm) {
    gdelData _constant = strtod(this->parser.previous.start, nullptr);
    emitConstant(_vm, _constant);
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
        default:
            return; // Unreachable.
    }
}

void gdelCompiler::binary(gdelVm& _vm) {
    gdelTokenType operatorType = this->parser.previous.type;
    gdelParseRule* rule = getParseRule(operatorType);
    parserPrecedence(_vm, (gdelPrecedence)(rule->precedence + 1));

    switch (operatorType) {
        case gdelTokenType::PLUS:          emitByte(gdelOpCode::OP_ADD);     break;
        case gdelTokenType::MINUS:         emitByte(gdelOpCode::OP_SUB);     break;
        case gdelTokenType::STAR:          emitByte(gdelOpCode::OP_MUL);     break;
        case gdelTokenType::SLASH:         emitByte(gdelOpCode::OP_DIV);     break;
        case gdelTokenType::PERCENTAGE:    emitByte(gdelOpCode::OP_PERCENT); break;
        case gdelTokenType::POW:           emitByte(gdelOpCode::OP_POW); break;
        default: return; // Unreachable.
    }
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

void gdelCompiler::emitReturn() {
    emitByte(gdelOpCode::OP_RETURN);
}

void gdelCompiler::endCompiler() {
    emitReturn();
}
