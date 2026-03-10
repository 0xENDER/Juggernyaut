
// Generated from f:/development/projects/juggernyaut/compiler/parser/.antlr4/JuggernyautParser.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"




class  JuggernyautParser : public antlr4::Parser {
public:
  enum {
    TMP = 1, CHARS_IGNORE_LIST = 2, COMMENT_BLOCK = 3, COMMENT_LINE = 4, 
    SYM_NEWLINE = 5, SYM_SEMICOLON = 6
  };

  enum {
    RuleStart = 0, RuleExpression = 1
  };

  explicit JuggernyautParser(antlr4::TokenStream *input);

  JuggernyautParser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~JuggernyautParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class StartContext;
  class ExpressionContext; 

  class  StartContext : public antlr4::ParserRuleContext {
  public:
    StartContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EOF();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> SYM_NEWLINE();
    antlr4::tree::TerminalNode* SYM_NEWLINE(size_t i);
    std::vector<antlr4::tree::TerminalNode *> SYM_SEMICOLON();
    antlr4::tree::TerminalNode* SYM_SEMICOLON(size_t i);

   
  };

  StartContext* start();

  class  ExpressionContext : public antlr4::ParserRuleContext {
  public:
    ExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TMP();

   
  };

  ExpressionContext* expression();


  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

