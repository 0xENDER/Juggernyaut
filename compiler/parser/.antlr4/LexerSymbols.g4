/**
 *
 * This is the ANTLR grammar file for the Juggernyaut Lexer!
 *
 * (Symbols)
 *
**/

lexer grammar LexerSymbols;

// Options
options {
    language=Cpp;
}

// New lines
SYM_NEWLINE
    : [\n]
    ; // Newline characters need to have their own tokens!

SYM_SEMICOLON
    : ';'
    ;