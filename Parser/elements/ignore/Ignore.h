#ifndef IGNORE_H
#define IGNORE_H

#include "Parser/core/Declarations.h"

class Ignore {
    public:
        void assignMethods(std::map<MatchCharFunc, TokenizerFunc>& _map);
};

#endif //IGNORE_H