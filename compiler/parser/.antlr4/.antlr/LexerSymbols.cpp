
// Generated from f:/development/projects/juggernyaut/compiler/parser/.antlr4/LexerSymbols.g4 by ANTLR 4.13.1


#include "LexerSymbols.h"


using namespace antlr4;



using namespace antlr4;

namespace {

struct LexerSymbolsStaticData final {
  LexerSymbolsStaticData(std::vector<std::string> ruleNames,
                          std::vector<std::string> channelNames,
                          std::vector<std::string> modeNames,
                          std::vector<std::string> literalNames,
                          std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), channelNames(std::move(channelNames)),
        modeNames(std::move(modeNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  LexerSymbolsStaticData(const LexerSymbolsStaticData&) = delete;
  LexerSymbolsStaticData(LexerSymbolsStaticData&&) = delete;
  LexerSymbolsStaticData& operator=(const LexerSymbolsStaticData&) = delete;
  LexerSymbolsStaticData& operator=(LexerSymbolsStaticData&&) = delete;

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

::antlr4::internal::OnceFlag lexersymbolsLexerOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
LexerSymbolsStaticData *lexersymbolsLexerStaticData = nullptr;

void lexersymbolsLexerInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (lexersymbolsLexerStaticData != nullptr) {
    return;
  }
#else
  assert(lexersymbolsLexerStaticData == nullptr);
#endif
  auto staticData = std::make_unique<LexerSymbolsStaticData>(
    std::vector<std::string>{
      "SYM_NEWLINE", "SYM_SEMICOLON"
    },
    std::vector<std::string>{
      "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
    },
    std::vector<std::string>{
      "DEFAULT_MODE"
    },
    std::vector<std::string>{
      "", "", "';'"
    },
    std::vector<std::string>{
      "", "SYM_NEWLINE", "SYM_SEMICOLON"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,0,2,9,6,-1,2,0,7,0,2,1,7,1,1,0,1,0,1,1,1,1,0,0,2,1,1,3,2,1,0,1,1,0,
  	10,10,8,0,1,1,0,0,0,0,3,1,0,0,0,1,5,1,0,0,0,3,7,1,0,0,0,5,6,7,0,0,0,6,
  	2,1,0,0,0,7,8,5,59,0,0,8,4,1,0,0,0,1,0,0
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  lexersymbolsLexerStaticData = staticData.release();
}

}

LexerSymbols::LexerSymbols(CharStream *input) : Lexer(input) {
  LexerSymbols::initialize();
  _interpreter = new atn::LexerATNSimulator(this, *lexersymbolsLexerStaticData->atn, lexersymbolsLexerStaticData->decisionToDFA, lexersymbolsLexerStaticData->sharedContextCache);
}

LexerSymbols::~LexerSymbols() {
  delete _interpreter;
}

std::string LexerSymbols::getGrammarFileName() const {
  return "LexerSymbols.g4";
}

const std::vector<std::string>& LexerSymbols::getRuleNames() const {
  return lexersymbolsLexerStaticData->ruleNames;
}

const std::vector<std::string>& LexerSymbols::getChannelNames() const {
  return lexersymbolsLexerStaticData->channelNames;
}

const std::vector<std::string>& LexerSymbols::getModeNames() const {
  return lexersymbolsLexerStaticData->modeNames;
}

const dfa::Vocabulary& LexerSymbols::getVocabulary() const {
  return lexersymbolsLexerStaticData->vocabulary;
}

antlr4::atn::SerializedATNView LexerSymbols::getSerializedATN() const {
  return lexersymbolsLexerStaticData->serializedATN;
}

const atn::ATN& LexerSymbols::getATN() const {
  return *lexersymbolsLexerStaticData->atn;
}




void LexerSymbols::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  lexersymbolsLexerInitialize();
#else
  ::antlr4::internal::call_once(lexersymbolsLexerOnceFlag, lexersymbolsLexerInitialize);
#endif
}
