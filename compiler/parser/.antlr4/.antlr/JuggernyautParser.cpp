
// Generated from f:/development/projects/juggernyaut/compiler/parser/.antlr4/JuggernyautParser.g4 by ANTLR 4.13.1



#include "JuggernyautParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct JuggernyautParserStaticData final {
  JuggernyautParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  JuggernyautParserStaticData(const JuggernyautParserStaticData&) = delete;
  JuggernyautParserStaticData(JuggernyautParserStaticData&&) = delete;
  JuggernyautParserStaticData& operator=(const JuggernyautParserStaticData&) = delete;
  JuggernyautParserStaticData& operator=(JuggernyautParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag juggernyautparserParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
JuggernyautParserStaticData *juggernyautparserParserStaticData = nullptr;

void juggernyautparserParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (juggernyautparserParserStaticData != nullptr) {
    return;
  }
#else
  assert(juggernyautparserParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<JuggernyautParserStaticData>(
    std::vector<std::string>{
      "start", "expression"
    },
    std::vector<std::string>{
      "", "'TMP'", "", "", "", "", "';'"
    },
    std::vector<std::string>{
      "", "TMP", "CHARS_IGNORE_LIST", "COMMENT_BLOCK", "COMMENT_LINE", "SYM_NEWLINE", 
      "SYM_SEMICOLON"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,6,25,2,0,7,0,2,1,7,1,1,0,1,0,4,0,7,8,0,11,0,12,0,8,1,0,4,0,12,8,0,
  	11,0,12,0,13,5,0,16,8,0,10,0,12,0,19,9,0,1,0,1,0,1,1,1,1,1,1,0,0,2,0,
  	2,0,1,1,0,5,6,26,0,17,1,0,0,0,2,22,1,0,0,0,4,6,3,2,1,0,5,7,7,0,0,0,6,
  	5,1,0,0,0,7,8,1,0,0,0,8,6,1,0,0,0,8,9,1,0,0,0,9,16,1,0,0,0,10,12,7,0,
  	0,0,11,10,1,0,0,0,12,13,1,0,0,0,13,11,1,0,0,0,13,14,1,0,0,0,14,16,1,0,
  	0,0,15,4,1,0,0,0,15,11,1,0,0,0,16,19,1,0,0,0,17,15,1,0,0,0,17,18,1,0,
  	0,0,18,20,1,0,0,0,19,17,1,0,0,0,20,21,5,0,0,1,21,1,1,0,0,0,22,23,5,1,
  	0,0,23,3,1,0,0,0,4,8,13,15,17
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  juggernyautparserParserStaticData = staticData.release();
}

}

JuggernyautParser::JuggernyautParser(TokenStream *input) : JuggernyautParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

JuggernyautParser::JuggernyautParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  JuggernyautParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *juggernyautparserParserStaticData->atn, juggernyautparserParserStaticData->decisionToDFA, juggernyautparserParserStaticData->sharedContextCache, options);
}

JuggernyautParser::~JuggernyautParser() {
  delete _interpreter;
}

const atn::ATN& JuggernyautParser::getATN() const {
  return *juggernyautparserParserStaticData->atn;
}

std::string JuggernyautParser::getGrammarFileName() const {
  return "JuggernyautParser.g4";
}

const std::vector<std::string>& JuggernyautParser::getRuleNames() const {
  return juggernyautparserParserStaticData->ruleNames;
}

const dfa::Vocabulary& JuggernyautParser::getVocabulary() const {
  return juggernyautparserParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView JuggernyautParser::getSerializedATN() const {
  return juggernyautparserParserStaticData->serializedATN;
}


//----------------- StartContext ------------------------------------------------------------------

JuggernyautParser::StartContext::StartContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* JuggernyautParser::StartContext::EOF() {
  return getToken(JuggernyautParser::EOF, 0);
}

std::vector<JuggernyautParser::ExpressionContext *> JuggernyautParser::StartContext::expression() {
  return getRuleContexts<JuggernyautParser::ExpressionContext>();
}

JuggernyautParser::ExpressionContext* JuggernyautParser::StartContext::expression(size_t i) {
  return getRuleContext<JuggernyautParser::ExpressionContext>(i);
}

std::vector<tree::TerminalNode *> JuggernyautParser::StartContext::SYM_NEWLINE() {
  return getTokens(JuggernyautParser::SYM_NEWLINE);
}

tree::TerminalNode* JuggernyautParser::StartContext::SYM_NEWLINE(size_t i) {
  return getToken(JuggernyautParser::SYM_NEWLINE, i);
}

std::vector<tree::TerminalNode *> JuggernyautParser::StartContext::SYM_SEMICOLON() {
  return getTokens(JuggernyautParser::SYM_SEMICOLON);
}

tree::TerminalNode* JuggernyautParser::StartContext::SYM_SEMICOLON(size_t i) {
  return getToken(JuggernyautParser::SYM_SEMICOLON, i);
}


size_t JuggernyautParser::StartContext::getRuleIndex() const {
  return JuggernyautParser::RuleStart;
}


JuggernyautParser::StartContext* JuggernyautParser::start() {
  StartContext *_localctx = _tracker.createInstance<StartContext>(_ctx, getState());
  enterRule(_localctx, 0, JuggernyautParser::RuleStart);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(17);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 98) != 0)) {
      setState(15);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case JuggernyautParser::TMP: {
          setState(4);
          expression();
          setState(6); 
          _errHandler->sync(this);
          alt = 1;
          do {
            switch (alt) {
              case 1: {
                    setState(5);
                    _la = _input->LA(1);
                    if (!(_la == JuggernyautParser::SYM_NEWLINE

                    || _la == JuggernyautParser::SYM_SEMICOLON)) {
                    _errHandler->recoverInline(this);
                    }
                    else {
                      _errHandler->reportMatch(this);
                      consume();
                    }
                    break;
                  }

            default:
              throw NoViableAltException(this);
            }
            setState(8); 
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx);
          } while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER);
          break;
        }

        case JuggernyautParser::SYM_NEWLINE:
        case JuggernyautParser::SYM_SEMICOLON: {
          setState(11); 
          _errHandler->sync(this);
          alt = 1;
          do {
            switch (alt) {
              case 1: {
                    setState(10);
                    _la = _input->LA(1);
                    if (!(_la == JuggernyautParser::SYM_NEWLINE

                    || _la == JuggernyautParser::SYM_SEMICOLON)) {
                    _errHandler->recoverInline(this);
                    }
                    else {
                      _errHandler->reportMatch(this);
                      consume();
                    }
                    break;
                  }

            default:
              throw NoViableAltException(this);
            }
            setState(13); 
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx);
          } while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER);
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(19);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(20);
    match(JuggernyautParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpressionContext ------------------------------------------------------------------

JuggernyautParser::ExpressionContext::ExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* JuggernyautParser::ExpressionContext::TMP() {
  return getToken(JuggernyautParser::TMP, 0);
}


size_t JuggernyautParser::ExpressionContext::getRuleIndex() const {
  return JuggernyautParser::RuleExpression;
}


JuggernyautParser::ExpressionContext* JuggernyautParser::expression() {
  ExpressionContext *_localctx = _tracker.createInstance<ExpressionContext>(_ctx, getState());
  enterRule(_localctx, 2, JuggernyautParser::RuleExpression);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(22);
    match(JuggernyautParser::TMP);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

void JuggernyautParser::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  juggernyautparserParserInitialize();
#else
  ::antlr4::internal::call_once(juggernyautparserParserOnceFlag, juggernyautparserParserInitialize);
#endif
}
