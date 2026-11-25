/*
   Project    : Source-to-HTML
   Author     : Vangimalla Naveen Kumar Reddy
   Batch ID   : 26015_092
   Language   : C

   DESCRIPTION
   -----------
        A C utility that reads a C source file, tokenizes it, and generates a syntax-highlighted HTML page 
    viewable in any browser. Pure C — no Flex, no Lex, no external libraries.

   USAGE
   -----
       $ gcc main.c token.c html.c type.c
       $ ./a.out <source.c> <output.html>

   SAMPLE INPUT (sample.c)
   -----------------------
       #include <stdio.h>
       int main(void)
       {
           int x = 42;
           char c = 'A';
           // Print
           printf("x=%d\n", x);
           return 0;
       }

   SAMPLE OUTPUT (in browser)
   --------------------------
       1  #include <stdio.h>
       2  int main(void)
       3  {
       4      int x = 42;
       5      char c = 'A';
       6      // Print
       7      printf("x=%d\n", x);
       8      return 0;
       9  }

       Colors: keywords blue, numbers green, strings orange, comments green italic, line numbers gray.

   FILES
   -----
       main.c   - driver, argument parsing
       token.c  - lexer (tokenizers for all types)
       type.c   - keyword table, CSS mapping
       html.c   - HTML writer (header, footer, escape, CSS)
       main.h   - structs, enums, prototypes

   TOKEN TYPES
   -----------
       KEYWORD, IDENTIFIER, NUMBER, FLOAT, STRING, CHAR,
       OPERATOR, PUNCTUATION, COMMENT, PREPROCESSOR,
       WHITESPACE, NEWLINE, UNKNOWN, ERROR, EOF

   HIGHLIGHTS
   ----------
       - Pure C, no external libraries
       - Self-contained HTML with embedded CSS
       - Line numbers via CSS counters (no JavaScript)
       - Hover highlighting per line
       - VS Code Dark+ theme
       - Works offline — one file, anywhere
 
*/

#include<stdio.h>
#include "main.h"

int main(int argc,char* argv[])
{
    // Check for correct number of command-line arguments
    if(argc!=3)
    {
        printf("Usage: %s <source_file> <output_file>\n", argv[0]);
        return failure;
    }

    // Get source and output file paths from command-line arguments
    char *src_path = argv[1];
    char *out_path = argv[2];
    Lexer lx;
    Token tok;
    
    // Open the source file for reading
    FILE *src = fopen(src_path, "r");
    if (!src) {
        fprintf(stderr, "Cannot open %s\n", src_path);
        fprintf(stderr, "Error processing source file.\n");
        return failure;
    }
    // Open the output file for writing
    FILE *out = fopen(out_path, "w");
    if (!out) {
        fprintf(stderr, "Cannot create %s\n", out_path);
        fclose(src);
        fprintf(stderr, "Error processing Output file.\n");
        return failure;
    }
    // Write the HTML header with CSS and title
    html_write_header(out, src_path);
    // Initialize the lexer with the source file
    lexer_init(&lx, src);

    // Tokenize the input source code and write each token to the output HTML file
    do {
        tok = tokenizer(&lx);
        html_write_token(out, &tok);
        token_free(&tok);
    } while (tok.type != TOKEN_EOF);

    // Write the HTML footer to close the document
    html_write_footer(out);

    fclose(src);
    fclose(out);

    printf("HTML output generated successfully: %s\n", out_path);
    return success;
}
