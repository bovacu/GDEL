#include <iostream>

#include "Parser/tests/ParserTests.hpp"
#include "Parser/include/Parser.hpp"

#include "Vm/include/common.h"
#include "Vm/include/memBlock.h"
#include "Vm/include/debug.h"
#include "Vm/include/vm.h"

void runParser();
void runInterpreter(const json& _ast);
void runVirtualMachine();

int main(int argc, char *argv[]) {
    
    runVirtualMachine();

    // fort::utf8_table _t;
    // fort::utf8_table _t0;
    // fort::utf8_table _t1;

    // _t.cur_cell().set_cell_text_style(fort::text_style::bold);
    // _t << fort::header << "T0";
    // _t.cur_cell().set_cell_text_style(fort::text_style::bold);
    // _t << "T1" << fort::endr;
    // // _t0.cur_cell().set_cell_text_style(fort::text_style::bold);
    // // _t0.cur_cell().set_cell_content_fg_color(fort::color::light_yellow);
    // _t0 << fort::header << "value1";
    // // _t0.cur_cell().set_cell_text_style(fort::text_style::bold);
    // // _t0.cur_cell().set_cell_content_fg_color(fort::color::light_yellow);
    // _t0 <<  "value2";
    // // _t0.cur_cell().set_cell_text_style(fort::text_style::bold);
    // // _t0.cur_cell().set_cell_content_fg_color(fort::color::light_yellow);
    // _t0 << "value3" << fort::endr;

    // // _t1.cur_cell().set_cell_text_style(fort::text_style::bold);
    // _t1 << fort::header << "value1";
    // // _t1.cur_cell().set_cell_text_style(fort::text_style::bold);
    // _t1 << "value2" << fort::endr;

    // _t0  << "11" << "12" << "13" << fort::endr;
    // _t0  << "11" << "12" << "13" << fort::endr;
    // _t1 << "23" << "24" << fort::endr;
    // _t1 << "26" << "25" << fort::endr;
    // _t1 << "26" << "25" << fort::endr;
    // _t1 << "26" << "25" << fort::endr;
    // _t1 << "26" << "25" << fort::endr;
	
    // _t.set_border_style(FT_EMPTY_STYLE);
    // _t0.set_border_style(FT_DOUBLE2_STYLE);
    // _t1.set_border_style(FT_DOUBLE2_STYLE);

    // for(auto _c = 0; _c < _t.col_count(); _c++){
    //     _t.column(_c).set_cell_text_align(fort::text_align::center);
    // }

    // // _t0.row(0).set_cell_content_fg_color(fort::color::light_yellow);
    // // _t1.row(0).set_cell_content_fg_color(fort::color::light_yellow);

    // _t << _t0.to_string() << _t1.to_string() << fort::endr;

    // std::cout << _t.to_string() << std::endl;

    return 0;
}

void runVirtualMachine() {
    initGdelVm();

    gdelMemBlock _memBlock;
    initGdelMemBlock(&_memBlock);

    size_t _constantDir = writeConstantToGdelMemBlock(&_memBlock, 16);
    size_t _constantDir2 = writeConstantToGdelMemBlock(&_memBlock, 75);
    writeToGdelMemBlock(&_memBlock, gdelOpCode::OP_CONST);
    writeToGdelMemBlock(&_memBlock, _constantDir);
    writeToGdelMemBlock(&_memBlock, gdelOpCode::OP_CONST);
    writeToGdelMemBlock(&_memBlock, _constantDir2);
    writeToGdelMemBlock(&_memBlock, gdelOpCode::OP_RETURN);

    runGdelVm(&_memBlock);

    freeGdelVm();
    freeGdleMemBlock(&_memBlock);
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



