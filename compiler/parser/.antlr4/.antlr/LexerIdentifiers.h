
// Generated from f:/development/projects/juggernyaut/compiler/parser/.antlr4/LexerIdentifiers.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"




class  LexerIdentifiers : public antlr4::Lexer {
public:
  enum {
    RULE_IDENTIFIER = 1, FUNCTION_IDENTIFIER = 2, TYPE_CONSTANT_IDENTIFIER = 3, 
    CONSTANT_IDENTIFIER = 4, TYPE_VARIABLE_IDENTIFIER = 5, VARIABLE_IDENTIFIER = 6, 
    LONG_FLAG_IDENTIFIER = 7, FLAG_IDENTIFIER = 8, TYPE_IDENTIFIER = 9, 
    COMMAND_IDENTIFIER = 10
  };

  explicit LexerIdentifiers(antlr4::CharStream *input);

  ~LexerIdentifiers() override;


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

