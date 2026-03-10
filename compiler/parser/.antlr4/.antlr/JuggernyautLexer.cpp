
// Generated from f:/development/projects/juggernyaut/compiler/parser/.antlr4/JuggernyautLexer.g4 by ANTLR 4.13.1


#include "JuggernyautLexer.h"


using namespace antlr4;



using namespace antlr4;

namespace {

struct JuggernyautLexerStaticData final {
  JuggernyautLexerStaticData(std::vector<std::string> ruleNames,
                          std::vector<std::string> channelNames,
                          std::vector<std::string> modeNames,
                          std::vector<std::string> literalNames,
                          std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), channelNames(std::move(channelNames)),
        modeNames(std::move(modeNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  JuggernyautLexerStaticData(const JuggernyautLexerStaticData&) = delete;
  JuggernyautLexerStaticData(JuggernyautLexerStaticData&&) = delete;
  JuggernyautLexerStaticData& operator=(const JuggernyautLexerStaticData&) = delete;
  JuggernyautLexerStaticData& operator=(JuggernyautLexerStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> channelNames;
  const std::vector<std::string> modeNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag juggernyautlexerLexerOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
JuggernyautLexerStaticData *juggernyautlexerLexerStaticData = nullptr;

void juggernyautlexerLexerInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (juggernyautlexerLexerStaticData != nullptr) {
    return;
  }
#else
  assert(juggernyautlexerLexerStaticData == nullptr);
#endif
  auto staticData = std::make_unique<JuggernyautLexerStaticData>(
    std::vector<std::string>{
      "TMP", "CHARS_IGNORE_LIST", "COMMENT_BLOCK", "COMMENT_LINE", "WHITESPACE_", 
      "SYM_NEWLINE", "SYM_SEMICOLON"
    },
    std::vector<std::string>{
      "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
    },
    std::vector<std::string>{
      "DEFAULT_MODE"
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
  	4,0,6,54,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,
  	1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,2,1,2,1,2,1,2,5,2,28,8,2,10,2,12,2,
  	31,9,2,1,2,1,2,1,2,1,2,1,2,1,3,1,3,1,3,1,3,5,3,42,8,3,10,3,12,3,45,9,
  	3,1,3,1,3,1,4,1,4,1,5,1,5,1,6,1,6,1,29,0,7,1,1,3,2,5,3,7,4,9,0,11,5,13,
  	6,1,0,2,1,0,10,10,3,0,9,9,13,13,32,32,54,0,1,1,0,0,0,0,3,1,0,0,0,0,5,
  	1,0,0,0,0,7,1,0,0,0,0,11,1,0,0,0,0,13,1,0,0,0,1,15,1,0,0,0,3,19,1,0,0,
  	0,5,23,1,0,0,0,7,37,1,0,0,0,9,48,1,0,0,0,11,50,1,0,0,0,13,52,1,0,0,0,
  	15,16,5,84,0,0,16,17,5,77,0,0,17,18,5,80,0,0,18,2,1,0,0,0,19,20,3,9,4,
  	0,20,21,1,0,0,0,21,22,6,1,0,0,22,4,1,0,0,0,23,24,5,47,0,0,24,25,5,42,
  	0,0,25,29,1,0,0,0,26,28,9,0,0,0,27,26,1,0,0,0,28,31,1,0,0,0,29,30,1,0,
  	0,0,29,27,1,0,0,0,30,32,1,0,0,0,31,29,1,0,0,0,32,33,5,42,0,0,33,34,5,
  	47,0,0,34,35,1,0,0,0,35,36,6,2,0,0,36,6,1,0,0,0,37,38,5,47,0,0,38,39,
  	5,47,0,0,39,43,1,0,0,0,40,42,8,0,0,0,41,40,1,0,0,0,42,45,1,0,0,0,43,41,
  	1,0,0,0,43,44,1,0,0,0,44,46,1,0,0,0,45,43,1,0,0,0,46,47,6,3,0,0,47,8,
  	1,0,0,0,48,49,7,1,0,0,49,10,1,0,0,0,50,51,7,0,0,0,51,12,1,0,0,0,52,53,
  	5,59,0,0,53,14,1,0,0,0,3,0,29,43,1,6,0,0
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  juggernyautlexerLexerStaticData = staticData.release();
}

}

JuggernyautLexer::JuggernyautLexer(CharStream *input) : Lexer(input) {
  JuggernyautLexer::initialize();
  _interpreter = new atn::LexerATNSimulator(this, *juggernyautlexerLexerStaticData->atn, juggernyautlexerLexerStaticData->decisionToDFA, juggernyautlexerLexerStaticData->sharedContextCache);
}

JuggernyautLexer::~JuggernyautLexer() {
  delete _interpreter;
}

std::string JuggernyautLexer::getGrammarFileName() const {
  return "JuggernyautLexer.g4";
}

const std::vector<std::string>& JuggernyautLexer::getRuleNames() const {
  return juggernyautlexerLexerStaticData->ruleNames;
}

const std::vector<std::string>& JuggernyautLexer::getChannelNames() const {
  return juggernyautlexerLexerStaticData->channelNames;
}

const std::vector<std::string>& JuggernyautLexer::getModeNames() const {
  return juggernyautlexerLexerStaticData->modeNames;
}

const dfa::Vocabulary& JuggernyautLexer::getVocabulary() const {
  return juggernyautlexerLexerStaticData->vocabulary;
}

antlr4::atn::SerializedATNView JuggernyautLexer::getSerializedATN() const {
  return juggernyautlexerLexerStaticData->serializedATN;
}

const atn::ATN& JuggernyautLexer::getATN() const {
  return *juggernyautlexerLexerStaticData->atn;
}




void JuggernyautLexer::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  juggernyautlexerLexerInitialize();
#else
  ::antlr4::internal::call_once(juggernyautlexerLexerOnceFlag, juggernyautlexerLexerInitialize);
#endif
}
