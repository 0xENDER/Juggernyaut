
// Generated from f:/development/projects/juggernyaut/compiler/parser/.antlr4/LexerIgnores.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"




class  LexerIgnores : public antlr4::Lexer {
public:
  enum {
    CHARS_IGNORE_LIST = 1, COMMENT_BLOCK = 2, COMMENT_LINE = 3
  };

  explicit LexerIgnores(antlr4::CharStream *input);

  ~LexerIgnores() override;


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

