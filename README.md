# Source-to-HTML (C Syntax Highlighter)

A lightweight, pure C command-line utility that reads a C source file, tokenizes it using a custom lexical analyzer, escapes HTML special characters, and generates a self-contained, syntax-highlighted HTML document styled with a **VS Code Dark+** theme. Built completely from scratch with zero external dependencies (no Flex, Lex, or JavaScript required).

## Table of Contents

* [Overview](#overview)
* [Features](#features)
* [Architecture & How It Works](#architecture--how-it-works)
* [Project Structure](#project-structure)
* [Supported Token Types & Styling](#supported-token-types--styling)
* [Getting Started](#getting-started)
  * [Prerequisites](#prerequisites)
  * [Compilation](#compilation)
  * [Usage](#usage)
* [Sample Input & Output](#sample-input--output)
* [Error Handling](#error-handling)
* [Author](#author)

## Overview

**Source-to-HTML** transforms plain `.c` source files into clean, readable, web-ready `.html` documents. Unlike heavy highlighters that rely on client-side JavaScript or external parser generators, this project implements its own single-pass character-by-character lexer in standard C and embeds all styling directly into the generated HTML output.

## Features

* **Pure Standard C:** Built using only standard C headers (`stdio.h`, `stdlib.h`, `string.h`, `ctype.h`)—no Flex, Bison, Lex, or third-party libraries.
* **Custom Lexical Analyzer:** Scans source code with 1-character lookahead (`lx_next`) and accurate line/column tracking (`lx_get`).
* **Comprehensive Token Recognition:**
  * All standard C keywords (`auto`, `break`, `case`, `char`, `const`, `struct`, `typedef`, `while`, etc.)
  * Identifiers and function/variable names
  * Integer, floating-point, and scientific-notation literals (e.g., `42`, `3.14`, `1e-5`)
  * Double-quoted string literals with escape sequences (`\"`, `\\`, `\n`)
  * Single-quoted character literals with escape sequences (`'A'`, `'\n'`, `'\''`)
  * Single-line (`// ...`) and multi-line (`/* ... */`) comments
  * Single-character and multi-character operators (`+`, `++`, `==`, `!=`, `&&`, `||`, `->`, `<<`, `>>`, etc.)
  * Punctuation delimiters (`(`, `)`, `{`, `}`, `[`, `]`, `;`, `,`)
  * Preprocessor directives (`#`)
* **Lexical Error Highlighting:** Detects and flags malformed tokens (such as unterminated strings, unterminated character literals, and invalid escape sequences) directly in the rendered HTML output.
* **Safe HTML Entity Escaping:** Converts `<`, `>`, `&`, `"`, and `'` into `&lt;`, `&gt;`, `&amp;`, `&quot;`, and `&#39;` to prevent HTML injection or broken markup.
* **Self-Contained VS Code Dark+ Output:**
  * Standalone `.html` file that works offline in any browser
  * Automatic line numbering via pure CSS `counter-increment` (zero JavaScript)
  * Subtle hover highlighting on each code line
  * Responsive viewport meta tags and monospace font stack (`Consolas`, `Menlo`, `Courier New`)

## Architecture & How It Works

1. **Initialization (`main.c`):** Validates command-line arguments (`<source_file> <output_file>`), opens file streams, writes the HTML header and embedded stylesheet, and initializes the `Lexer` state.
2. **Lexical Analysis (`token.c`):** `tokenizer()` reads characters via `lx_get()` and peeks ahead with `lx_next()`, dispatching to specialized scanners (`scan_alpha`, `scan_number`, `scan_string`, `scan_char`, `scan_comment`) to produce `Token` structs with dynamically allocated lexemes.
3. **Classification & Styling (`type.c`):** `is_keyword()` checks identifiers against C's reserved keyword table, and `token_css_class()` maps each `TokenType` to its corresponding CSS class name.
4. **HTML Emission (`html.c`):** `html_write_token()` wraps each token in a `<span class="...">` element, escapes special HTML characters via `html_escape()`, and splits lines cleanly using `<span class="line">`.
5. **Cleanup (`main.c` & `token.c`):** Frees each token's heap-allocated lexeme via `token_free()`, writes the closing HTML tags, and closes open file descriptors.

## Project Structure

```text
├── main.c       # CLI argument validation, file I/O, and main lexer-to-HTML driver loop
├── token.c      # Custom lexer engine, lookahead helpers, and specialized token scanners
├── type.c       # C keyword lookup table and token-to-CSS class mapping
├── html.c       # Embedded VS Code Dark+ CSS, HTML entity escaper, and HTML writers
├── main.h       # TokenType enum, Token & Lexer structs, macros, and function prototypes
└── makefile     # Build configuration to compile object files and link ./a.out
```

## Supported Token Types & Styling

| Token Type | CSS Class | Visual Style | Description |
| :--- | :--- | :--- | :--- |
| `TOKEN_KEYWORD` | `.keyword` | Bold Blue (`#439ee8`) | Reserved C keywords |
| `TOKEN_IDENTIFIER` | `.identifier` | Light Blue (`#78afcc`) | Variable, function, struct, and macro names |
| `TOKEN_NUMBER` | `.number` | Soft Green (`#b5cea8`) | Integer literals |
| `TOKEN_FLOAT` | `.number` | Soft Green (`#b5cea8`) | Decimal and scientific-notation literals |
| `TOKEN_STRING` | `.string` | Peach/Orange (`#ce9178`) | Double-quoted string literals |
| `TOKEN_CHAR` | `.string` | Peach/Orange (`#ce9178`) | Single-quoted character literals |
| `TOKEN_COMMENT` | `.comment` | Italic Green (`#6a9955`) | Single-line (`//`) and block (`/* */`) comments |
| `TOKEN_PREPROCESSOR` | `.preprocessor` | Purple/Pink (`#c586c0`) | Preprocessor `#` symbol |
| `TOKEN_OPERATOR` | `.operator` | Light Gray (`#d4d4d4`) | Arithmetic, relational, logical, bitwise, and `->` |
| `TOKEN_PUNCTUATION` | `.punct` | Light Gray (`#d4d4d4`) | Delimiters `()`, `{}`, `[]`, `;`, `,` |
| `TOKEN_WHITESPACE` | — | Preserved | Spaces, tabs, and carriage returns |
| `TOKEN_NEWLINE` | `.line` | Block wrapper | Closes current line span and starts next numbered line |
| `TOKEN_ERROR` | `.error` | Red badge (`#e70b0b`) | Unterminated strings, chars, or bad escapes |
| `TOKEN_UNKNOWN` | `.unknown` | Light Gray (`#d4d4d4`) | Any unrecognized characters |

## Getting Started

### Prerequisites

* **GCC** (or any C99/C11 compliant compiler)
* **Make** (optional, for automated building using `makefile`)

### Compilation

#### Option 1: Using `make`

```bash
make
```

This compiles `main.o`, `token.o`, `html.o`, and `type.o` and links them into `./a.out`.

#### Option 2: Manual Compilation with `gcc`

```bash
gcc -Wall -Wextra main.c token.c html.c type.c -o source2html
```

### Usage

```bash
./a.out <source_file.c> <output_file.html>
```

#### Example

```bash
./a.out sample.c output.html
```

Console output:

```text
HTML output generated successfully: output.html
```

Open `output.html` in any web browser (Chrome, Firefox, Edge, Safari) to view your formatted code.

## Sample Input & Output

### Input (`sample.c`)

```c
#include <stdio.h>

int main(void)
{
    int x = 42;
    char c = 'A';
    // Print value
    printf("x=%d\n", x);
    return 0;
}
```

### Rendered HTML Output

When opened in a browser, the generated HTML displays:

* A dark editor background (`#1e1e1e`) with a rounded code container (`#252526`).
* Right-aligned line numbers (`1` through `10`) separated by a subtle vertical border.
* Interactive row highlighting when hovering over any line of code.
* Distinct syntax coloring for preprocessor directives, keywords, identifiers, numbers, character/string literals, and comments.

## Error Handling

The utility handles both file-level and lexical-level errors gracefully:

* **Invalid Arguments:** Displays usage instructions (`Usage: ./a.out <source_file> <output_file>`) if fewer or more than 2 file paths are provided.
* **File I/O Errors:** Reports clear messages to `stderr` if the source file cannot be opened or the output file cannot be created, cleaning up open handles before exiting.
* **Lexical Errors:** Unterminated strings (`"hello`), unterminated character literals (`'a`), or invalid escapes are tokenized as `TOKEN_ERROR` and highlighted in red in the generated HTML instead of crashing the parser.

## Author

* **Vangimalla Naveen Kumar Reddy**
  * **Language:** C
  * **Email:** [vangimallanaveenkumarreddy1@gmail.com](mailto:vangimallanaveenkumarreddy1@gmail.com)
  * **LinkedIn:** [Naveenkumar Reddy Vangimalla](https://www.linkedin.com/in/naveenkumar-reddy-vangimalla-310184415/)
