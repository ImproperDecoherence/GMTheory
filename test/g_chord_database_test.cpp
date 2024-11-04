#include <iostream>

#include "g_chord_database.hpp"
#include "g_test_framework.hpp"

using namespace std;
using namespace gbase;
using namespace gmtheory;

namespace test {

GTEST(GChordDatabaseTest) {
    GLocalLogLevel localLogLevel(GLogger::LogLevel::None);

    const GChordDatabase &db = GChordDatabase::getInstance();
    GCHECK("Database instance", db.size() > 0, true);

    const GChord chordCdim(NoteName{"C"}, GChordType::Augmented, CMF::NoFlag);
    const Size distance0{0};
    const GSet<GChord> expectedMatchedChordsD0 = {GChord(NoteName{"C"}, GChordType::Augmented, CMF::NoFlag),
                                                  GChord(NoteName{"E"}, GChordType::Augmented, CMF::NoFlag),
                                                  GChord(NoteName{"Ab"}, GChordType::Augmented, CMF::NoFlag)};
    const GSet<GChord> matchCdim = db.match(chordCdim.noteValues(), distance0);

    GCHECK("Match distance 0", matchCdim, expectedMatchedChordsD0);

    const Size distance1{1};
    const GChord chordC(NoteName{"C"}, GChordType::Major, CMF::NoFlag);
    const GSet<GChord> expectedMatchedChordsD1 = {GChord(NoteName{"C"}, GChordType::Major, CMF::Major7),
                                                  GChord(NoteName{"C"}, GChordType::Major, CMF::Dominant7),
                                                  GChord(NoteName{"C"}, GChordType::Major, CMF::Add2),
                                                  GChord(NoteName{"C"}, GChordType::Major, CMF::Add6),
                                                  GChord(NoteName{"C"}, GChordType::Major, CMF::Add9)};
    const GSet<GChord> matchC = db.match(chordC.noteValues(), distance1);

    GCHECK("Match distance 1", expectedMatchedChordsD1.isSubsetOf(matchC), true);
}

} // namespace test