
// Generated from f:/development/projects/juggernyaut/compiler/parser/.antlr4/LexerLiterals.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"




class  LexerLiterals : public antlr4::Lexer {
public:
  enum {
    LIT_BOOLEAN_TRUE = 1, LIT_BOOLEAN_FALSE = 2, LIT_NUMBER = 3, LIT_STRING_START = 4, 
    LIT_STRING_CONTENT_ESCAPED = 5, INVALID_LIT_STRING_REFERENCE = 6, LIT_STRING_REFERENCE_TYPE_CONSTANT = 7, 
    LIT_STRING_REFERENCE_TYPE_VARIABLE = 8, LIT_STRING_REFERENCE_CONSTANT = 9, 
    LIT_STRING_REFERENCE_VARIABLE = 10, LIT_STRING_CONTENT = 11, LIT_STRING_END = 12
  };

  enum {
    MODE_STRING_CAPTURE = 1
  };

  explicit LexerLiterals(antlr4::CharStream *input);

  ~LexerLiterals() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

};

