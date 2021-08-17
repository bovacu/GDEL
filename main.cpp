#include <iostream>

#include "Parser/tests/ParserTests.hpp"
#include "Parser/include/Parser.hpp"

#include "Vm/include/common.h"
#include "Vm/include/memBlock.h"
#include "Vm/include/debug.h"
#include "Vm/include/vm.h"

static char* readFile(gdelVm& _vm, const char* path) {
    FILE* file = fopen(path, "rb");

    if (file == NULL) {
        fprintf(stderr, "Could not open file \"%s\".\n", path);
        exit(74); 
    } 

    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);
    char* buffer = (char*)malloc(fileSize + 1);

    if (buffer == NULL) {
        fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
        exit(74); 
    }

    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);

    if (bytesRead < fileSize) {
        fprintf(stderr, "Could not read file \"%s\".\n", path);
        exit(74); 
    }

    buffer[bytesRead] = '\0';
    fclose(file);
    return buffer;
}

static void runFile(gdelVm& _vm, const char* path) {
    char* source = readFile(_vm, path);
    gdelProgramResult result = _vm.run(source);
    free(source);
    if (result == gdelProgramResult::PROGRAM_COMPILE_ERROR) exit(65);
    if (result == gdelProgramResult::PROGRAM_RUNTIME_ERROR) exit(70);
}

static void repl(gdelVm& _vm) {
    char _line[1024];
    for (;;) {
        printf("gdel~ ");
        std::cin.getline(_line, sizeof(_line));
        if(strcmp(_line, "exit()") == 0)
            break;

        // _vm.run(_line);
    }
}

void runParser();
void runInterpreter(const json& _ast);
void runVirtualMachine();

int main(int argc, char *argv[]) {
    
    gdelVm _vm;
    _vm.init();

    if(argc == 1) {
        repl(_vm);
    } else if (argc == 2) {
        runFile(_vm, argv[1]);
    } else {
        std::cerr << "Usage: gdel [path]" << std::endl;
        exit(64);
    }

    _vm.end();

    return 0;
}

void runVirtualMachine() {

    gdelMemBlock _memBlock;
    initGdelMemBlock(&_memBlock);

    size_t _constantDir = writeConstantToGdelMemBlock(&_memBlock, 2.2);
    size_t _constantDir2 = writeConstantToGdelMemBlock(&_memBlock, 3.4);
    size_t _constantDir3 = writeConstantToGdelMemBlock(&_memBlock, 5.6);
    writeToGdelMemBlock(&_memBlock, gdelOpCode::OP_CONST);
    writeToGdelMemBlock(&_memBlock, _constantDir);
    writeToGdelMemBlock(&_memBlock, gdelOpCode::OP_CONST);
    writeToGdelMemBlock(&_memBlock, _constantDir2);
    writeToGdelMemBlock(&_memBlock, gdelOpCode::OP_ADD);
    writeToGdelMemBlock(&_memBlock, gdelOpCode::OP_CONST);
    writeToGdelMemBlock(&_memBlock, _constantDir3);
    writeToGdelMemBlock(&_memBlock, gdelOpCode::OP_DIV);
    writeToGdelMemBlock(&_memBlock, gdelOpCode::OP_NEGATE);
    writeToGdelMemBlock(&_memBlock, gdelOpCode::OP_RETURN);

    // runGdelVm(&_memBlock);

    
}

void runParser() {
    #ifdef RUN_TESTS
        ParserTests _pt;
        _pt.run();
    #else
        try {
            auto _root = parse(R"(
                'hellooo';
                var _z = 'hello \'escapado\' oh yeah';
                var y = 'hello {'formatted'} with any value {-123} nice';
            )");
            std::cout << _root.dump(4) << std::endl;
            runInterpreter(_root);
        } catch(CoreException e) {  }
        catch(const char *e) { std::cerr << e << std::endl;}
    #endif
}

void runInterpreter(const json& _ast) {

}