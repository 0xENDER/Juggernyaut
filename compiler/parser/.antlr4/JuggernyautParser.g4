/**
 *
 * This is the ANTLR grammar file for Juggernyaut!
 * All syntax and parser rules are defined here.
 *
 * (Only contains finalised implementations!)
 *
 * (ANTLR4: https://github.com/antlr/antlr4/blob/master/doc/lexicon.md#keywords)
 *
**/

parser grammar JuggernyautParser;

// Manage options
options {
    tokenVocab=JuggernyautLexer;
    language=Cpp;
}

//// Parser Rules

start
    :   (
            expression (SYM_NEWLINE | SYM_SEMICOLON)+
            |
            (SYM_NEWLINE | SYM_SEMICOLON)+
        )* EOF
    ; /* This is the start scope! */

// Expressions
expression
    : TMP //TMP
    ; /* All supported expressions */
