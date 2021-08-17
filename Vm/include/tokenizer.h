#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__

#include "dataStructs/TrieDS.hpp"

typedef enum {
    // Single-character s.
    LEFT_PAREN, RIGHT_PAREN,
    LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS,
    SEMICOLON, SLASH, STAR,
    PERCENTAGE, LEFT_BRACKET,
    RIGHT_BRACKET, POW,

    // One or two character s.
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

// Literals.
    IDENTIFIER, STRING, NUMBER, BOOL,

// Keywords.
    AND, REG, ELSE, FALSE,
    FOR, FUNC, IF, NULL_, OR,
    PRINT, RET, BASE, SELF,
    TRUE, VAR, LOOP,
    ERROR,
    EOF_
  } gdelTokenType;

typedef struct {
    gdelTokenType type;
    const char* start;
    int length;
    int line;
} gdelToken;

class gdelTokenizer {
    private:
        const char* start;
        const char* current;
        int line;
        TrieDS trie;

    public:
        void init(const char* _code);
        gdelToken scanToken();
        gdelToken makeToken(gdelTokenType _tokenType);
        gdelToken errorToken(const char* _error);

    private:
        void initTrieDS();
        bool isAtEnd();
        char advance();
        char peek();
        char peekNext();
        void skipWhitespaces();
        bool match(char _expected);
        bool isDigit(char _digit);
        bool isAlpha(char _alpha);

        gdelToken string();
        gdelToken number();
        gdelToken identifier();

        gdelTokenType identifierType();
};

#endif // __TOKENIZER_H__