#include "main.h"
 
// Function to initialize the lexer with a source file
void lexer_init(Lexer *lx, FILE *src)
{
    lx->src      = src;
    lx->line     = 1;
    lx->column   = 1;
    lx->next     = 0;
    lx->has_next = 0;
}
// Function to tokenize the input source code and return the next token
Token tokenizer(Lexer* lx)
{
    char buf[MAX_LEXEME];
    int len=0;
    int ch;
    int line=lx->line;
    int col=lx->column;

    ch=lx_get(lx);
    // Handle whitespace, newlines, and end-of-file
    if (ch == ' ' || ch == '\t' || ch == '\r') {
        buf[len++] = (char)ch;
        while (lx_next(lx) == ' ' || lx_next(lx) == '\t' || lx_next(lx) == '\r') {
            if (len >= MAX_LEXEME - 1) break;
            buf[len++] = (char)lx_get(lx);
        }
        buf[len] = '\0';
        return make_token(TOKEN_WHITESPACE, buf, line, col);
    }
    // Handle end-of-file, newlines, and preprocessor directives
    if( ch == EOF)
    {
        return make_token(TOKEN_EOF,"<EOF>",line,col);
    }
    // Handle newlines and preprocessor directives
    if (ch == '\n') {
        return make_token(TOKEN_NEWLINE, "\n", line, col);
    }
    // Handle preprocessor directives starting with '#'
    if (ch == '#') {
        buf[len++] = (char)ch;
        buf[len] = '\0';
        return make_token(TOKEN_PREPROCESSOR, buf, line, col);
    }
    // Handle identifiers, keywords, numbers, strings, characters, operators, and punctuation
    if( isalpha(ch) || ch=='_')
    {
        buf[len++]=ch;
        return scan_alpha(lx,buf ,&len,line,col);
    }
    // Handle numbers, strings, comments, and character literals
    if( isdigit(ch))
    {
        buf[len++]=ch;
        return scan_number(lx,buf,&len,line,col);
    }
    // Handle string literals starting with '"'
    if(ch=='"')
    {
        return scan_string(lx,buf,&len,line,col);
    }
    // Handle comments starting with '/' and character literals starting with '\''
    if (ch == '/') {
        int n = lx_next(lx);
        if (n == '/' || n == '*') 
        {
            buf[len++] = (char)ch; 
            return scan_comment(lx, buf, &len, line, col);
        }
        buf[len++] = (char)lx_get(lx);
        buf[len] = '\0';
        return make_token(TOKEN_OPERATOR, buf, line, col);
    }
    // Handle character literals starting with '\''
    if (ch == '\'')
    {
        return scan_char(lx, buf, &len, line, col);
    }
    // Handle operators and punctuation
    if(ch == '=' || ch == '!' || ch == '>' || ch == '<'|| ch == '&' || ch=='|' || ch == '+' || ch == '-' || ch == '*' || ch == '%')
    {
            int n= lx_next(lx);
            int two=0;
            if(n=='=') two=1;
            if(ch=='&' && n=='&') two=1;
            if (ch == '|' && n == '|') two = 1;
            if (ch == '+' && n == '+') two = 1;
            if (ch == '-' && n == '-') two = 1;
            if (ch == '-' && n == '>') two = 1;
            if (ch == '<' && n == '<') two = 1;
            if (ch == '>' && n == '>') two = 1;

            if(two)
            {
                buf[0]=(char)ch;
                buf[1]=(char)n;
                buf[2]='\0';
            }
            else
            {
                buf[0]=(char)ch;
                buf[1]='\0';
            }
            return make_token(TOKEN_OPERATOR,buf,line,col);
    }
    // Handle other operators and punctuation
    if (ch == '~' || ch == '^') 
    {
        buf[0] = (char)ch; 
        buf[1] = '\0';
        return make_token(TOKEN_OPERATOR, buf, line, col);
    }
    // Handle punctuation characters
    if (strchr("(){}[];,", ch)) 
    {
        buf[0] = (char)ch; 
        buf[1] = '\0';
        return make_token(TOKEN_PUNCTUATION, buf, line, col);
    }

    buf[0] = (char)ch; 
    buf[1] = '\0';
    return make_token(TOKEN_UNKNOWN, buf, line, col);
}

// Build a Token by duplicating the lexeme and recording type, line and column
Token make_token(TokenType t,char *s,int line,int col )
{ 
    Token tok;
    tok.type=t;
    tok.lexeme=strdup(s);
    tok.column=col;
    tok.line=line;
    return tok;
}

int lx_get(Lexer *lx)
{
    // Consume one character and update line/column counters (newline resets column to 1)
    int c= lx->has_next == 1? lx->next : fgetc(lx->src);
    lx->has_next=0;
    if(c=='\n')
    {
        lx->line++;
        lx->column=1;
    }
    else if(c!= EOF)
    {
        lx->column++;
    }
    return c;
}
// Peek at the next character without consuming it
Token scan_alpha(Lexer*lx,char*buff,int *len,int line,int col)
{
    while(isalnum(lx_next(lx))|| lx_next(lx)=='_')
    {
        if(*len >= MAX_LEXEME-1)
        break;
        buff[(*len)++]=lx_get(lx);
    }
    buff[*len]='\0';
    TokenType t= is_keyword(buff)? TOKEN_KEYWORD :  TOKEN_IDENTIFIER;
    return make_token(t,buff,line,col);
}

// Peek at the next character without consuming it
int lx_next(Lexer*lx)
{
    if(lx->has_next == 0)
    {
        lx->next=fgetc(lx->src);
        lx->has_next=1;
    }
    return lx->next;
}

// Scan a number literal, handling integers, floats, and scientific notation
Token scan_number(Lexer* lx,char*buf,int *len,int line,int col)
{
    int is_float=0;
    while(isdigit(lx_next(lx)) && *len< MAX_LEXEME-1)
    {
        buf[(*len)++]=lx_get(lx);
    }
    if(lx_next(lx)=='.' && *len< MAX_LEXEME-2)
    {
        is_float=1;
        buf[(*len)++]=lx_get(lx);
        while(isdigit(lx_next(lx)) && *len<MAX_LEXEME-1)
            buf[(*len)++]=lx_get(lx);
    }
    if(lx_next(lx)== 'e' || lx_next(lx) == 'E')
    {
        is_float = 1;
        buf[(*len)++]=lx_get(lx);
        if(lx_next(lx)=='+' || lx_next(lx)=='-')
            buf[(*len)++]= lx_get(lx);
        while(isdigit(lx_next(lx)) && *len< MAX_LEXEME -1)
            buf[(*len)++]= lx_get(lx);
    }
    
    buf[(*len)++]='\0';
    Token tok = make_token(is_float ? TOKEN_FLOAT : TOKEN_NUMBER,buf, line, col);
    return tok;
}

// Scan a string literal, handling backslash escapes and unterminated strings
Token scan_string(Lexer* lx,char*buf,int*len,int line,int col)
{
    buf[(*len)++]='"';
    int c;
    while((c =lx_get(lx)) != EOF && c != '"' )
    {
        if(c=='\n')
        {
            buf[*len]='\0';
            return make_token(TOKEN_ERROR, "unterminated string", line, col);
        }
        if(*len >= MAX_LEXEME -3)
            break;
        buf[(*len)++]=(char)c;
        if(c=='\\')
        {
            int n=lx_get(lx);
            if(n == EOF)
                break;
            if(*len < MAX_LEXEME-2)
            buf[(*len)++]= (char)n;
        }
    }
    if(c=='"')
    buf[(*len)++]='"';
    buf[(*len)++]='\0';
    return make_token(TOKEN_STRING,buf,line,col);
}

// Scan a comment, handling both single-line and multi-line comments
Token scan_comment(Lexer *lx, char *buf, int *len, int line, int col)
{
    buf[(*len)++] = (char)lx_get(lx);  

    if (buf[1] == '/') {
        while (lx_next(lx) != '\n' && lx_next(lx) != EOF) {
            if (*len >= MAX_LEXEME - 1) break;
            buf[(*len)++] = (char)lx_get(lx);
        }
    } else {
        int prev = 0;
        while (lx_next(lx) != EOF) {
            int c = lx_get(lx);
            if (*len >= MAX_LEXEME - 1) break;
            buf[(*len)++] = (char)c;
            if (prev == '*' && c == '/') break;
            prev = c;
        }
    }
    buf[*len] = '\0';
    return make_token(TOKEN_COMMENT, buf, line, col);
}

// Scan a single-quoted character literal, handling backslash escapes
Token scan_char(Lexer*lx,char *buf,int *len,int line,int col)
{
    // Scan a single-quoted character literal, handling backslash escapes
    buf[(*len)++]='\'';
    int c=lx_get(lx);
    if(c==EOF || c== '\n')
    {
        return make_token(TOKEN_ERROR,"Unterminated Character",line,col);
    }
    buf[(*len)++]= (char)c;
    if(c=='\\')
    {
        int n=lx_get(lx);
        if(n== EOF)
            return make_token(TOKEN_ERROR,"Bad Escpe",line,col);
        buf[(*len)++]=(char)n;
    }
    if(lx_next(lx) != '\'')
        return make_token(TOKEN_ERROR,"Unterminated Char",line,col);
    
    buf[(*len)++]=(char)lx_get(lx);
    buf[(*len)++]='\0';
    return make_token(TOKEN_CHAR,buf,line,col);
}

// Free the memory allocated for a token's lexeme
void token_free(Token* tok)
{
    if(tok && tok->lexeme)
    {
        free(tok->lexeme);
        tok->lexeme =NULL;
    }
}