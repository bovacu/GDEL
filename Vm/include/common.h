#ifndef __COMMON_H__
#define __COMMON_H__

#include <iostream>
#include <stdint.h>
#include <stdlib.h>

// #define DEBUG_EXECUTION

#define EXIT_NO_POSSIBLE_REALLOCATION_ERROR -1

#define PRINT_ERROR(_message) std::cout << "\033[1;31m" << _message << "\033[0m";
#define PRINT_LN_ERROR(_message) std::cout << "\033[1;31m" << _message << "\033[0m" << std::endl



typedef uint8_t byte;

#endif // __COMMON_H__