#include "Vm/include/tokenizer.h"
#include <string.h>

void gdelTokenizer::init(const char* _code) {
    this->current = _code;
    this->start = _code;
    this->line = 1;
}

char gdelTokenizer::advance() {
    this->current++;
    return this->current[-1];
}

char gdelTokenizer::peek() {
    return *this->current;
}

char gdelTokenizer::peekNext() {
    if (isAtEnd()) return '\0';
    return this->current[1];
}

void gdelTokenizer::skipWhitespaces() {
    for (;;) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                this->line++;
                advance();
                break;
            case '#':
                while (peek() != '\n' && !isAtEnd()) advance();
          break;
            default:
                return;
        } 
    }
}

bool gdelTokenizer::match(char expected) {
    if (isAtEnd()) return false;
    if (*this->current != expected) return false;
    this->current++;
    return true;
}

gdelToken gdelTokenizer::string() {
    while (peek() != '\'' && !isAtEnd()) {
        if (peek() == '\n') this->line++;
            advance(); 
        }
    if (isAtEnd()) return errorToken("Unterminated string.");
    // The closing quote.
    advance();
    return makeToken(gdelTokenType::STRING);
}

bool gdelTokenizer::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool gdelTokenizer::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
            c == '_';
}

gdelTokenType gdelTokenizer::checkKeyword(int start, int length, const char* rest, gdelTokenType type) {
    if (this->current - this->start == start + length && memcmp(this->start + start, rest, length) == 0) {
      return type;
    }
    return gdelTokenType::IDENTIFIER;
}

gdelTokenType gdelTokenizer::identifierType() {
    switch (this->start[0]) {
        case 'a': return checkKeyword(1, 2, "nd", gdelTokenType::AND);
        case 'b': return checkKeyword(1, 4, "base", gdelTokenType::SUPER);
        case 'c': return checkKeyword(1, 4, "lass", gdelTokenType::CLASS);
        case 'e': return checkKeyword(1, 3, "lse", gdelTokenType::ELSE);
        case 'i': return checkKeyword(1, 1, "f", gdelTokenType::IF);
        case 'f':
            if (this->current - this->start > 1) {
                switch (this->start[1]) {
                    case 'a': return checkKeyword(2, 3, "lse", gdelTokenType::FALSE);
                    case 'o': return checkKeyword(2, 1, "r", gdelTokenType::FOR);
                    case 'u': return checkKeyword(2, 2, "nc", gdelTokenType::FUN);
                } 
            }
        break;
        case 'l': return checkKeyword(1, 3, "oop", gdelTokenType::WHILE);
        case 'n': return checkKeyword(1, 2, "il", gdelTokenType::NIL);
        case 'o': return checkKeyword(1, 1, "r", gdelTokenType::OR);
        case 'p': return checkKeyword(1, 4, "rint", gdelTokenType::PRINT);
        case 'r': return checkKeyword(1, 2, "et", gdelTokenType::RETURN);
        case 's': return checkKeyword(1, 3, "elf", gdelTokenType::THIS);
        case 't': return checkKeyword(1, 3, "rue", gdelTokenType::THIS);
        case 'v': return checkKeyword(1, 2, "ar", gdelTokenType::VAR);
    }
    return gdelTokenType::IDENTIFIER;
}

gdelToken gdelTokenizer::identifier() {
    while (isAlpha(peek()) || isDigit(peek())) advance();
    return makeToken(identifierType());
}

gdelToken gdelTokenizer::number() {
    while (isDigit(peek())) advance();
    // Look for a fractional part.
    if (peek() == '.' && isDigit(peekNext())) {
        // Consume the ".".
        advance();

        // TODO implement ranges here
        if(peek() == '.') {
            
        }

        while (isDigit(peek())) advance();
    }
    return makeToken(gdelTokenType::NUMBER);
}

gdelToken gdelTokenizer::scanToken() {

    skipWhitespaces();

    this->start = this->current;
    if (isAtEnd()) return makeToken(gdelTokenType::EOF_);
    
    char c = advance();
    if (isAlpha(c)) return identifier();
    if (isDigit(c)) return number();

    switch (c) {
        case '(': return makeToken(gdelTokenType::LEFT_PAREN);
        case ')': return makeToken(gdelTokenType::RIGHT_PAREN);
        case '{': return makeToken(gdelTokenType::LEFT_BRACE);
        case '}': return makeToken(gdelTokenType::RIGHT_BRACE);
        case '[': return makeToken(gdelTokenType::LEFT_BRACKET);
        case ']': return makeToken(gdelTokenType::RIGHT_BRACKET);
        case ';': return makeToken(gdelTokenType::SEMICOLON);
        case ',': return makeToken(gdelTokenType::COMMA);
        case '.': return makeToken(gdelTokenType::DOT);
        case '-': return makeToken(gdelTokenType::MINUS);
        case '+': return makeToken(gdelTokenType::PLUS);
        case '/': return makeToken(gdelTokenType::SLASH);
        case '*': return makeToken(gdelTokenType::STAR);
        case '^': return makeToken(gdelTokenType::POW);
        case '%': return makeToken(gdelTokenType::PERCENTAGE);
        case '!': return makeToken(match('=') ? gdelTokenType::BANG_EQUAL : gdelTokenType::BANG);
        case '=': return makeToken(match('=') ? gdelTokenType::EQUAL_EQUAL : gdelTokenType::EQUAL);
        case '<': return makeToken(match('=') ? gdelTokenType::LESS_EQUAL : gdelTokenType::LESS);
        case '>': return makeToken(match('=') ? gdelTokenType::GREATER_EQUAL : gdelTokenType::GREATER);
        case '\'': return string();
    }

    return errorToken("Unexpected character.");
}

bool gdelTokenizer::isAtEnd() {
    return *this->current == '\0';
}

gdelToken gdelTokenizer::makeToken(gdelTokenType _tokenType) {
    gdelToken token;
    token.type = _tokenType;
    token.start = this->start;
    token.length = (int)(this->current - this->start);
    token.line = this->line;
    return token;
}

gdelToken gdelTokenizer::errorToken(const char* _error) {
    gdelToken token;
    token.type = gdelTokenType::ERROR;
    token.start = _error;
    token.length = (int)strlen(_error);
    token.line = this->line;
    return token;
}