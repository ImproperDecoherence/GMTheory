#include <sstream>

#include "gdictionary.hpp"
#include "gexceptions.hpp"
#include "testframework.hpp"

using namespace gbase;

namespace test {

GTEST(GDictionaryTest) {
    const GDictionary<Integer, String> d1{{7, "seven"}, {11, "eleven"}, {21, "twentyone"}};

    GCHECK("Const access 1", d1[7], String("seven"));
    GCHECK("Const access 1", d1[21], String("twentyone"));

    GCHECK("Size", d1.size(), Size{3});

    auto d2{d1};
    d2[11] = "ELEVEN";
    GCHECK("Non-const access", d2[11], String("ELEVEN"));

    bool outOfRange{false};
    try {
        auto value = d1[77];
    } catch (GOutOfRange &exep) {
        outOfRange = true;
    } catch (...) {
    }
    GCHECK("Out of range", outOfRange, true);

    std::stringstream ss{""};
    ss << d1;
    const String expectedPrint{"[7: seven, 11: eleven, 21: twentyone]"};
    GCHECK("Stream", ss.str(), expectedPrint);

    auto v1 = d1.keyVector();
    const GVector<Integer> expectedKeys{7, 11, 21};
    GCHECK("Key vector", v1, expectedKeys);

    auto v2 = d1.valueVector();
    const GVector<String> expectedValues = {String{"seven"}, String{"eleven"}, String{"twentyone"}};
    GCHECK("Key vector", v2, expectedValues);
}

} // namespace test