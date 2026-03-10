
// Generated from f:/development/projects/juggernyaut/compiler/parser/.antlr4/LexerIdentifiersFragments.g4 by ANTLR 4.13.1


#include "LexerIdentifiersFragments.h"


using namespace antlr4;



using namespace antlr4;

namespace {

struct LexerIdentifiersFragmentsStaticData final {
  LexerIdentifiersFragmentsStaticData(std::vector<std::string> ruleNames,
                          std::vector<std::string> channelNames,
                          std::vector<std::string> modeNames,
                          std::vector<std::string> literalNames,
                          std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), channelNames(std::move(channelNames)),
        modeNames(std::move(modeNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  LexerIdentifiersFragmentsStaticData(const LexerIdentifiersFragmentsStaticData&) = delete;
  LexerIdentifiersFragmentsStaticData(LexerIdentifiersFragmentsStaticData&&) = delete;
  LexerIdentifiersFragmentsStaticData& operator=(const LexerIdentifiersFragmentsStaticData&) = delete;
  LexerIdentifiersFragmentsStaticData& operator=(LexerIdentifiersFragmentsStaticData&&) = delete;

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

::antlr4::internal::OnceFlag lexeridentifiersfragmentsLexerOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
LexerIdentifiersFragmentsStaticData *lexeridentifiersfragmentsLexerStaticData = nullptr;

void lexeridentifiersfragmentsLexerInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (lexeridentifiersfragmentsLexerStaticData != nullptr) {
    return;
  }
#else
  assert(lexeridentifiersfragmentsLexerStaticData == nullptr);
#endif
  auto staticData = std::make_unique<LexerIdentifiersFragmentsStaticData>(
    std::vector<std::string>{
      "CONSTANT_IDENTIFIER_CONTENT_", "VARIABLE_IDENTIFIER_CONTENT_", "STANDARD_IDENTIFIER_CHARS_", 
      "STANDARD_IDENTIFIER_CHARS_START_", "STANDARD_IDENTIFIER_CHARS_END_", 
      "STANDARD_IDENTIFIER_CHARS_CONSTANTS_", "STANDARD_IDENTIFIER_CHARS_CONSTANTS_START_", 
      "STANDARD_IDENTIFIER_CHARS_CONSTANTS_END_", "DIGIT_", "WHITESPACE_", 
      "ESCAPE_SEQUENCE_"
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
  	4,0,0,62,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,
  	2,7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,1,0,1,0,5,0,26,8,0,10,0,12,0,29,9,0,
  	1,0,3,0,32,8,0,1,1,1,1,5,1,36,8,1,10,1,12,1,39,9,1,1,1,3,1,42,8,1,1,2,
  	1,2,1,3,1,3,1,4,1,4,1,5,1,5,1,6,1,6,1,7,1,7,1,8,1,8,1,9,1,9,1,10,1,10,
  	1,10,0,0,11,1,0,3,0,5,0,7,0,9,0,11,0,13,0,15,0,17,0,19,0,21,0,1,0,9,5,
  	0,45,45,48,57,65,90,95,95,97,122,3,0,65,90,95,95,97,122,4,0,48,57,65,
  	90,95,95,97,122,4,0,45,45,48,57,65,90,95,95,2,0,65,90,95,95,3,0,48,57,
  	65,90,95,95,1,0,48,57,3,0,9,9,13,13,32,32,8,0,34,34,36,36,47,47,92,92,
  	98,98,102,102,109,110,114,116,54,1,23,1,0,0,0,3,33,1,0,0,0,5,43,1,0,0,
  	0,7,45,1,0,0,0,9,47,1,0,0,0,11,49,1,0,0,0,13,51,1,0,0,0,15,53,1,0,0,0,
  	17,55,1,0,0,0,19,57,1,0,0,0,21,59,1,0,0,0,23,31,3,13,6,0,24,26,3,11,5,
  	0,25,24,1,0,0,0,26,29,1,0,0,0,27,25,1,0,0,0,27,28,1,0,0,0,28,30,1,0,0,
  	0,29,27,1,0,0,0,30,32,3,15,7,0,31,27,1,0,0,0,31,32,1,0,0,0,32,2,1,0,0,
  	0,33,41,3,7,3,0,34,36,3,5,2,0,35,34,1,0,0,0,36,39,1,0,0,0,37,35,1,0,0,
  	0,37,38,1,0,0,0,38,40,1,0,0,0,39,37,1,0,0,0,40,42,3,9,4,0,41,37,1,0,0,
  	0,41,42,1,0,0,0,42,4,1,0,0,0,43,44,7,0,0,0,44,6,1,0,0,0,45,46,7,1,0,0,
  	46,8,1,0,0,0,47,48,7,2,0,0,48,10,1,0,0,0,49,50,7,3,0,0,50,12,1,0,0,0,
  	51,52,7,4,0,0,52,14,1,0,0,0,53,54,7,5,0,0,54,16,1,0,0,0,55,56,7,6,0,0,
  	56,18,1,0,0,0,57,58,7,7,0,0,58,20,1,0,0,0,59,60,5,92,0,0,60,61,7,8,0,
  	0,61,22,1,0,0,0,5,0,27,31,37,41,0
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  lexeridentifiersfragmentsLexerStaticData = staticData.release();
}

}

LexerIdentifiersFragments::LexerIdentifiersFragments(CharStream *input) : Lexer(input) {
  LexerIdentifiersFragments::initialize();
  _interpreter = new atn::LexerATNSimulator(this, *lexeridentifiersfragmentsLexerStaticData->atn, lexeridentifiersfragmentsLexerStaticData->decisionToDFA, lexeridentifiersfragmentsLexerStaticData->sharedContextCache);
}

LexerIdentifiersFragments::~LexerIdentifiersFragments() {
  delete _interpreter;
}

std::string LexerIdentifiersFragments::getGrammarFileName() const {
  return "LexerIdentifiersFragments.g4";
}

const std::vector<std::string>& LexerIdentifiersFragments::getRuleNames() const {
  return lexeridentifiersfragmentsLexerStaticData->ruleNames;
}

const std::vector<std::string>& LexerIdentifiersFragments::getChannelNames() const {
  return lexeridentifiersfragmentsLexerStaticData->channelNames;
}

const std::vector<std::string>& LexerIdentifiersFragments::getModeNames() const {
  return lexeridentifiersfragmentsLexerStaticData->modeNames;
}

const dfa::Vocabulary& LexerIdentifiersFragments::getVocabulary() const {
  return lexeridentifiersfragmentsLexerStaticData->vocabulary;
}

antlr4::atn::SerializedATNView LexerIdentifiersFragments::getSerializedATN() const {
  return lexeridentifiersfragmentsLexerStaticData->serializedATN;
}

const atn::ATN& LexerIdentifiersFragments::getATN() const {
  return *lexeridentifiersfragmentsLexerStaticData->atn;
}




void LexerIdentifiersFragments::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  lexeridentifiersfragmentsLexerInitialize();
#else
  ::antlr4::internal::call_once(lexeridentifiersfragmentsLexerOnceFlag, lexeridentifiersfragmentsLexerInitialize);
#endif
}
