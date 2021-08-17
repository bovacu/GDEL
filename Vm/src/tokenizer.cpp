#include "Vm/include/tokenizer.h"
#include <string.h>
#include <iostream>

void gdelTokenizer::init(const char* _code) {
    this->current = _code;
    this->start = _code;
    this->line = 1;

    if(this->trie.getSize() == 0)
        initTrieDS();
}

void gdelTokenizer::initTrieDS() {
    trie.insertKey("and");
    trie.insertKey("base");
    trie.insertKey("else");
    trie.insertKey("elif");
    trie.insertKey("if");
    trie.insertKey("false");
    trie.insertKey("for");
    trie.insertKey("func");
    trie.insertKey("null");
    trie.insertKey("or");
    trie.insertKey("print");
    trie.insertKey("reg");
    trie.insertKey("self");
    trie.insertKey("true");
    trie.insertKey("var");
}

/*
 * Okay this can seem really estrange and incorrect as we are passing -1 to the index of the array, but the thing is in C++
 * we have this:
 * 
 *      int arr[4] = {3, 0, 5, 8};
 *      std::cout << arr[2] << std::endl; 
 * 
 * With this code we get 5 as output, but what happend to get this output? Well, arr[2] is just a redefinition of *(arr + 4),
 * and thats basic pointer arithmetic
 * 
 *      [    3    |    0    |    5    |    8    ]   ->  [    3    |    0    |    5    |    8    ]
 *           ^                                                                   ^
 *          arr                                                               arr + 2
 * 
 *  And of course we deference de address to get the value inside that pointer.
 * 
 * So, if we do current++, the pointer is already one char ahead, and by doing current[-1] = *(current - 1) we are getting the 
 * current char and the current pointer is pointing to the next one.
*/
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

gdelTokenType gdelTokenizer::identifierType() {
    switch (this->start[0]) {
        case 'a': if(this->trie.hasKey("and")) return gdelTokenType::AND;
        case 'b': if(this->trie.hasKey("base")) return gdelTokenType::BASE;
        case 'e': {
                    if(this->trie.hasKey("else")) return gdelTokenType::ELSE;
                    // if(this->trie.hasKey("elif")) return gdelTokenType::ELIF;
                }
        case 'i': if(this->trie.hasKey("if")) return gdelTokenType::IF;
        case 'f': {
                    if(this->trie.hasKey("false")) return gdelTokenType::FALSE;
                    if(this->trie.hasKey("func")) return gdelTokenType::FUNC;
                    if(this->trie.hasKey("for")) return gdelTokenType::FOR;
                }
        case 'l': if(this->trie.hasKey("loop")) return gdelTokenType::LOOP;
        case 'n': if(this->trie.hasKey("null")) return gdelTokenType::NULL_;
        case 'o': if(this->trie.hasKey("or")) return gdelTokenType::OR;
        case 'p': if(this->trie.hasKey("print")) return gdelTokenType::PRINT;
        case 'r': {
            if(this->trie.hasKey("reg")) return gdelTokenType::REG;
            if(this->trie.hasKey("ret")) return gdelTokenType::RET;
        }
        case 's': if(this->trie.hasKey("self")) return gdelTokenType::SELF;
        case 't': if(this->trie.hasKey("true")) return gdelTokenType::TRUE;
        case 'v': if(this->trie.hasKey("var")) return gdelTokenType::VAR;
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