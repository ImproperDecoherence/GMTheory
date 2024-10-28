
#include "gexceptions.hpp"
#include "glogger.hpp"
#include "gprinttools.hpp"
#include "testframework.hpp"
#include <iostream>

int main(int argc, char *argv[]) {

    gbase::GLogger &logger = gbase::GLogger::getInstance();
    logger.setLogLevel(gbase::GLogger::LogLevel::None);

    try {
        gtest::TestFramework::getInstance().executeTests();
    } catch (const gbase::GException &exception) {
        cout << gbase::CoutColor::Magenta << exception << endl;
    }

    return 0;
}
