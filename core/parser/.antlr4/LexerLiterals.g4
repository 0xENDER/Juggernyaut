/**
 *
 * This is the ANTLR grammar file for the Juggernyaut Lexer!
 *
 * (Literals)
 *
**/

lexer grammar LexerLiterals;

// Options
options {
    language=Cpp;
}

import LexerFragments;

LIT_STRING
    : '"' CHAIN_CHAR_* '"'
    ;
INVALID_LIT_STRING
    : '"' CHAIN_CHAR_* [\r\n]
    ;
