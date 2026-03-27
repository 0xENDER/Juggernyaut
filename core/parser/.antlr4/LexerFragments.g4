/**
 *
 * This is the ANTLR grammar file for the Juggernyaut Lexer!
 *
 * (Fragments)
 *
**/

lexer grammar LexerFragments;

// Options
options {
    language=Cpp;
}

// Ignored whitespace
fragment WHITESPACE_
    : [ \t\r\n]
    ;

// Strings & Chars
fragment ESCAPE_SEQUENCE_
    : '\\' [\r\n"\\]
    ; /* Escape characters */
fragment IMP_CHAR_
    : ESCAPE_SEQUENCE_
    | '\\\n' // newline escape
    | '\\\r\n' // newline escape
    ; /* Char member of a string */
fragment CHAIN_CHAR_
    : ~["\\\r\n]
    | IMP_CHAR_
    ; /* Char member of a string */
fragment LONE_CHAR_
    : ~['\\\r\n]
    | IMP_CHAR_
    ; /* Char member of a character */
