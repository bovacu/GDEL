#include "json.hpp"
#include <functional>
#define BIND_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

using namespace nlohmann;

class Parser;
class Statement;

using Func = std::function<json(const Statement&, Parser&, json&)>;

typedef json (*TokenizerFunc)(const char*, int&, int); // token, charPointer, linePointer
typedef bool (*MatchCharFunc)(const char*, int); // the second parameter is for the ones that need lookahead > 1, such as bool