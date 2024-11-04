#include <iostream>

#include "g_exceptions.hpp"
#include "g_logger.hpp"
#include "g_print_tools.hpp"
#include "g_test_framework.hpp"

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
