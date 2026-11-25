#include "main.h"

// Function to escape HTML special characters in a string
char *CSS =
"* { box-sizing: border-box; }"
"body { background: #1e1e1e; color: #d4d4d4;"
"  font-family: 'Consolas', 'Menlo', 'Courier New', monospace;"
"  font-size: 14px; padding: 20px; margin: 0; }"
"h1 { color: #e0e0e0; font-size: 18px; margin: 0 0 16px 0; }"
"pre { counter-reset: line; margin: 0; line-height: 1.5;"
"      background: #252526; padding: 16px; border-radius: 6px;"
"      overflow-x: auto; }"
"code { display: block; }"
".line { display: block; }"
".line::before { counter-increment: line; content: counter(line);"
"  color: #858585; display: inline-block; width: 3em;"
"  padding-right: 1em; text-align: right; user-select: none;"
"  border-right: 1px solid #444; margin-right: 1em; }"
".line:hover { background: #2a2d2e; }"
".keyword      { color: #439ee8; font-weight: bold; }"
".identifier   { color: #78afcc; }"
".number       { color: #b5cea8; }"
".string       { color: #ce9178; }"
".operator     { color: #d4d4d4; }"
".punct        { color: #d4d4d4; }"
".comment      { color: #6a9955; font-style: italic; }"
".preprocessor { color: #c586c0; }"
".error        { color: #e70b0b; background: #4b1113;"
"                padding: 0 4px; border-radius: 3px; }"
".unknown      { color: #d4d4d4; }";

// Function to escape HTML special characters in a string

void html_escape(FILE *fp, const char *s)
{
    for (const char *p = s; *p; p++) {
        switch (*p) {
            case '<':  fputs("&lt;",   fp); break;
            case '>':  fputs("&gt;",   fp); break;
            case '&':  fputs("&amp;",  fp); break;
            case '"':  fputs("&quot;", fp); break;
            case '\'': fputs("&#39;",  fp); break;
            default:   fputc(*p, fp);       break;
        }
    }
}

// Function to write a token as an HTML span with appropriate class
void html_write_token(FILE *fp, Token *tok)
{
    if (tok->type == TOKEN_EOF) 
        return;

    if (tok->type == TOKEN_NEWLINE) 
    {
        fputs("</span>\n<span class=\"line\">", fp);
        return;
    }

    if (tok->type == TOKEN_WHITESPACE) 
    {
        html_escape(fp, tok->lexeme);
        return;
    }

    fprintf(fp, "<span class=\"%s\">", token_css_class(tok->type));
    html_escape(fp, tok->lexeme);
    fputs("</span>", fp);
}
// Function to write the HTML header with CSS and title
void html_write_header(FILE *fp, const char *filename)
{
    fprintf(fp,"<!DOCTYPE html>\n <html lang=\"en\">\n"
        "<head>\n"
        "<meta charset=\"UTF-8\">\n"
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
        "<title>%s &mdash; Syntax Highlighted</title>\n"
        "<style>%s</style>\n"
        "</head>\n"
        "<body>\n"
        "<h1>&#128196; %s</h1>\n"
        "<pre><code><span class=\"line\">",
        filename, CSS, filename);
}
// Function to write the HTML footer
void html_write_footer(FILE *fp)
{
    fputs("</span></code></pre>\n</body>\n</html>\n", fp);
}