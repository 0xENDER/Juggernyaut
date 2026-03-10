
// Generated from f:/development/projects/juggernyaut/compiler/parser/.antlr4/LexerFragments.g4 by ANTLR 4.13.1


#include "LexerFragments.h"


using namespace antlr4;



using namespace antlr4;

namespace {

struct LexerFragmentsStaticData final {
  LexerFragmentsStaticData(std::vector<std::string> ruleNames,
                          std::vector<std::string> channelNames,
                          std::vector<std::string> modeNames,
                          std::vector<std::string> literalNames,
                          std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), channelNames(std::move(channelNames)),
        modeNames(std::move(modeNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  LexerFragmentsStaticData(const LexerFragmentsStaticData&) = delete;
  LexerFragmentsStaticData(LexerFragmentsStaticData&&) = delete;
  LexerFragmentsStaticData& operator=(const LexerFragmentsStaticData&) = delete;
  LexerFragmentsStaticData& operator=(LexerFragmentsStaticData&&) = delete;

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

::antlr4::internal::OnceFlag lexerfragmentsLexerOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
LexerFragmentsStaticData *lexerfragmentsLexerStaticData = nullptr;

void lexerfragmentsLexerInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (lexerfragmentsLexerStaticData != nullptr) {
    return;
  }
#else
  assert(lexerfragmentsLexerStaticData == nullptr);
#endif
  auto staticData = std::make_unique<LexerFragmentsStaticData>(
    std::vector<std::string>{
      "WHITESPACE_"
    },
    std::vector<std::string>{
      "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
    },
    std::vector<std::string>{
      "DEFAULT_MODE"
    },
    std::vector<std::string>{
    },
    std::vector<std::string>{
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,0,0,5,6,-1,2,0,7,0,1,0,1,0,0,0,1,1,0,1,0,1,3,0,9,9,13,13,32,32,3,1,
  	3,1,0,0,0,3,4,7,0,0,0,4,2,1,0,0,0,1,0,0
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  lexerfragmentsLexerStaticData = staticData.release();
}

}

LexerFragments::LexerFragments(CharStream *input) : Lexer(input) {
  LexerFragments::initialize();
  _interpreter = new atn::LexerATNSimulator(this, *lexerfragmentsLexerStaticData->atn, lexerfragmentsLexerStaticData->decisionToDFA, lexerfragmentsLexerStaticData->sharedContextCache);
}

LexerFragments::~LexerFragments() {
  delete _interpreter;
}

std::string LexerFragments::getGrammarFileName() const {
  return "LexerFragments.g4";
}

const std::vector<std::string>& LexerFragments::getRuleNames() const {
  return lexerfragmentsLexerStaticData->ruleNames;
}

const std::vector<std::string>& LexerFragments::getChannelNames() const {
  return lexerfragmentsLexerStaticData->channelNames;
}

const std::vector<std::string>& LexerFragments::getModeNames() const {
  return lexerfragmentsLexerStaticData->modeNames;
}

const dfa::Vocabulary& LexerFragments::getVocabulary() const {
  return lexerfragmentsLexerStaticData->vocabulary;
}

antlr4::atn::SerializedATNView LexerFragments::getSerializedATN() const {
  return lexerfragmentsLexerStaticData->serializedATN;
}

const atn::ATN& LexerFragments::getATN() const {
  return *lexerfragmentsLexerStaticData->atn;
}




void LexerFragments::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  lexerfragmentsLexerInitialize();
#else
  ::antlr4::internal::call_once(lexerfragmentsLexerOnceFlag, lexerfragmentsLexerInitialize);
#endif
}
