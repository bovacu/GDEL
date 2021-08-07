#ifndef __ISTATEMENT_H__
#define __ISTATEMENT_H__

#include "Parser/include/Declarations.h"

class IStatement {
    public:
        virtual inline Func getFunc() = 0;
};

#endif // __ISTATEMENT_H__