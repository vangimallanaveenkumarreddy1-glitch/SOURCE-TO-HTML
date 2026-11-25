#include "main.h"

// Function to escape HTML special characters in a string
char *token_css_class(TokenType t)
{
    switch (t) {
        case TOKEN_KEYWORD:      return "keyword";
        case TOKEN_IDENTIFIER:   return "identifier";
        case TOKEN_NUMBER:       return "number";
        case TOKEN_FLOAT:        return "number";
        case TOKEN_STRING:       return "string";
        case TOKEN_CHAR:         return "string";
        case TOKEN_OPERATOR:     return "operator";
        case TOKEN_PUNCTUATION:  return "punct";
        case TOKEN_COMMENT:      return "comment";
        case TOKEN_PREPROCESSOR: return "preprocessor";
        case TOKEN_ERROR:        return "error";
        default:                 return "unknown";
    }
}

// Function to check if a string is a C keyword
char *KEYWORDS[] = {
    "auto", "break", "case", "char", "const", "continue",
    "default", "do", "double", "else", "enum", "extern",
    "float", "for", "goto", "if", "inline", "int", "long",
    "register", "restrict", "return", "short", "signed",
    "sizeof", "static", "struct", "switch", "typedef",
    "union", "unsigned", "void", "volatile", "while",
    "_Bool", "_Complex", "_Imaginary", NULL
};

// Function to check if a string is a C keyword
int is_keyword(const char *s)
{
    for (int i = 0; KEYWORDS[i]; i++)
        if (strcmp(s, KEYWORDS[i]) == 0) return 1;
    return 0;
}