#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__

#include "dataStructs/TrieDS.hpp"

/*
 * This class is the classic Tokenizer, it loops through the code written by the user and separates the words into tokens.
 * The main method to this class (the one using in upper classes) is getToken(). 
 * 
 * getToken() basically takes the next word and transforms it to a specific token by some criteria, this is the order
 *      - First of all, skip white spaces (this includes /n, /t, /r, ' ' and comments). In case it is a comment, we skip all
 *        until getting the new line. If it is a white space different from /n, then we just advance the pointer and in case it is
 *        we increase the line counter as we are in a new line.
 *      - Once there are no more white spaces we check if it is the end of the code, in that case the token is EOF_ and just return.
 *      - If not EOF_, we get the token type by criteria and precedence:
 *          - number -> if it applies to [0-9] or [0-9].[0-9] we return NUMBER
 *          - identifier -> if it applies to [a-z][A-Z][_]+ we return IDENTIFIER
 *          - symbols -> if it is a symbol defined in our syntax, we return THE_SYMBOL_NAME
 *              - a particular case of symbol is string, if it starts with ' and ends with ' we return a STRING
 *      - If the character doesn't apply to any of this, it is an error and we return an errorToken.
*/

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
        gdelToken getToken();

    private:
        gdelToken makeToken(gdelTokenType _tokenType);
        gdelToken errorToken(const char* _error);
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