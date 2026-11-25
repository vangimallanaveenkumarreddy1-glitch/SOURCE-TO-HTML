#ifndef HEADER
#define HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define success -1
#define failure -2
#define MAX_LEXEME 4096

typedef enum token_type {
    TOKEN_EOF = 0,
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_FLOAT,
    TOKEN_STRING,
    TOKEN_CHAR,
    TOKEN_OPERATOR,
    TOKEN_PUNCTUATION,
    TOKEN_COMMENT,
    TOKEN_PREPROCESSOR,
    TOKEN_WHITESPACE,
    TOKEN_NEWLINE,
    TOKEN_UNKNOWN,
    TOKEN_ERROR,
    TOKEN_TYPE_COUNT
} TokenType;

typedef struct token {
    int type;
    char *lexeme;
    int line;
    int column;
} Token;

typedef struct lexer {
    FILE *src;
    int   line;
    int   column;
    int   next;
    int   has_next;
} Lexer;
 
void html_write_header(FILE *fp, const char *src_path);
void html_write_footer(FILE *fp);  
Token tokenizer(Lexer* lx);
Token make_token(TokenType t,char *s,int line,int col );
int lx_get(Lexer*lx);
int lx_next(Lexer*lx);
Token scan_alpha(Lexer*lx,char*buff,int *len,int line,int col);
Token scan_number(Lexer* lx,char*buf,int *len,int line,int col);
Token scan_string(Lexer* lx,char*buf,int*len,int line,int col);
Token scan_operator(Lexer* lx,char*buf,int *len,int line,int col);
char *token_css_class(TokenType t);
int is_keyword(const char *s);
void token_free(Token *tok);
void lexer_init(Lexer *lx, FILE *src);
void html_write_token(FILE *fp, Token *tok);
Token scan_comment(Lexer *lx, char *buf, int *len, int line, int col);
Token scan_char(Lexer*lx,char *buf,int *len,int line,int col);
#endif