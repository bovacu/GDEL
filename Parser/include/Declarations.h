#include "json.hpp"
using namespace nlohmann;

typedef json (*TokenizerFunc)(const char*, int&, int); // token, charPointer, linePointer
typedef bool (*MatchCharFunc)(const char*, int); // the second parameter is for the ones that need lookahead > 1, such as bool