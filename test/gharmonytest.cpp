#include <iostream>

#include "gharmony.hpp"
#include "testframework.hpp"

using namespace std;
using namespace gmtheory;
using namespace gbase;

namespace test {

GTEST(GHarmonyTest) {
    const GHarmony h1 = {0, 3, 7, 9};
    GHarmony h2{0, 3, 7, 9};
    GCHECK("Constructors", h1, h2);

    h2 += NoteValue{11};
    GHarmony h3 = h1 + NoteValue{11};
    GCHECK("Extend with note value", h2, h3);

    h2.erase(9);
    h3 -= NoteValue{9};
    GCHECK("Erase note value", h2, h3);

    h2 += {13, 15};
    h3 += GHarmony{13, 15};
    GCHECK("Extend with init list", h2, h3);

    GHarmony h4 = h2 + GHarmony{11, 17, 19};
    const GHarmony h5 = {0, 3, 7, 13, 15, 11, 17, 19};
    GCHECK("Addition", h4, h5);

    h4.erase(GHarmony{11, 17, 19, 21});
    const GHarmony h6 = h5 - GHarmony{11, 17, 19, 21};
    GCHECK("Subtraction", h4, h6);

    const GHarmony h7 = {"C0", "Eb0", "Ab0"};
    const GHarmony h0 = {0, 3, 8};
    GCHECK("Note name constructor", h4, h6);
}

} // namespace test