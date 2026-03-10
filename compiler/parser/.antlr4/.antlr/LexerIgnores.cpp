
// Generated from f:/development/projects/juggernyaut/compiler/parser/.antlr4/LexerIgnores.g4 by ANTLR 4.13.1


#include "LexerIgnores.h"


using namespace antlr4;



using namespace antlr4;

namespace {

struct LexerIgnoresStaticData final {
  LexerIgnoresStaticData(std::vector<std::string> ruleNames,
                          std::vector<std::string> channelNames,
                          std::vector<std::string> modeNames,
                          std::vector<std::string> literalNames,
                          std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), channelNames(std::move(channelNames)),
        modeNames(std::move(modeNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  LexerIgnoresStaticData(const LexerIgnoresStaticData&) = delete;
  LexerIgnoresStaticData(LexerIgnoresStaticData&&) = delete;
  LexerIgnoresStaticData& operator=(const LexerIgnoresStaticData&) = delete;
  LexerIgnoresStaticData& operator=(LexerIgnoresStaticData&&) = delete;

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

::antlr4::internal::OnceFlag lexerignoresLexerOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
LexerIgnoresStaticData *lexerignoresLexerStaticData = nullptr;

void lexerignoresLexerInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (lexerignoresLexerStaticData != nullptr) {
    return;
  }
#else
  assert(lexerignoresLexerStaticData == nullptr);
#endif
  auto staticData = std::make_unique<LexerIgnoresStaticData>(
    std::vector<std::string>{
      "CHARS_IGNORE_LIST", "COMMENT_BLOCK", "COMMENT_LINE", "WHITESPACE_"
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
      "", "CHARS_IGNORE_LIST", "COMMENT_BLOCK", "COMMENT_LINE"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,0,3,40,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,1,0,1,0,1,0,1,0,1,1,1,1,
  	1,1,1,1,5,1,18,8,1,10,1,12,1,21,9,1,1,1,1,1,1,1,1,1,1,1,1,2,1,2,1,2,1,
  	2,5,2,32,8,2,10,2,12,2,35,9,2,1,2,1,2,1,3,1,3,1,19,0,4,1,1,3,2,5,3,7,
  	0,1,0,2,1,0,10,10,3,0,9,9,13,13,32,32,40,0,1,1,0,0,0,0,3,1,0,0,0,0,5,
  	1,0,0,0,1,9,1,0,0,0,3,13,1,0,0,0,5,27,1,0,0,0,7,38,1,0,0,0,9,10,3,7,3,
  	0,10,11,1,0,0,0,11,12,6,0,0,0,12,2,1,0,0,0,13,14,5,47,0,0,14,15,5,42,
  	0,0,15,19,1,0,0,0,16,18,9,0,0,0,17,16,1,0,0,0,18,21,1,0,0,0,19,20,1,0,
  	0,0,19,17,1,0,0,0,20,22,1,0,0,0,21,19,1,0,0,0,22,23,5,42,0,0,23,24,5,
  	47,0,0,24,25,1,0,0,0,25,26,6,1,0,0,26,4,1,0,0,0,27,28,5,47,0,0,28,29,
  	5,47,0,0,29,33,1,0,0,0,30,32,8,0,0,0,31,30,1,0,0,0,32,35,1,0,0,0,33,31,
  	1,0,0,0,33,34,1,0,0,0,34,36,1,0,0,0,35,33,1,0,0,0,36,37,6,2,0,0,37,6,
  	1,0,0,0,38,39,7,1,0,0,39,8,1,0,0,0,3,0,19,33,1,6,0,0
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  lexerignoresLexerStaticData = staticData.release();
}

}

LexerIgnores::LexerIgnores(CharStream *input) : Lexer(input) {
  LexerIgnores::initialize();
  _interpreter = new atn::LexerATNSimulator(this, *lexerignoresLexerStaticData->atn, lexerignoresLexerStaticData->decisionToDFA, lexerignoresLexerStaticData->sharedContextCache);
}

LexerIgnores::~LexerIgnores() {
  delete _interpreter;
}

std::string LexerIgnores::getGrammarFileName() const {
  return "LexerIgnores.g4";
}

const std::vector<std::string>& LexerIgnores::getRuleNames() const {
  return lexerignoresLexerStaticData->ruleNames;
}

const std::vector<std::string>& LexerIgnores::getChannelNames() const {
  return lexerignoresLexerStaticData->channelNames;
}

const std::vector<std::string>& LexerIgnores::getModeNames() const {
  return lexerignoresLexerStaticData->modeNames;
}

const dfa::Vocabulary& LexerIgnores::getVocabulary() const {
  return lexerignoresLexerStaticData->vocabulary;
}

antlr4::atn::SerializedATNView LexerIgnores::getSerializedATN() const {
  return lexerignoresLexerStaticData->serializedATN;
}

const atn::ATN& LexerIgnores::getATN() const {
  return *lexerignoresLexerStaticData->atn;
}




void LexerIgnores::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  lexerignoresLexerInitialize();
#else
  ::antlr4::internal::call_once(lexerignoresLexerOnceFlag, lexerignoresLexerInitialize);
#endif
}
