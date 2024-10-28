#include <iostream>

#include "gchordmodifiers.hpp"
#include "gchords.hpp"
#include "gnotechars.hpp"
#include "testframework.hpp"

using namespace std;
using namespace gbase;
using namespace gmtheory;

namespace test {

GTEST(GChordTest) {
    GChord chordC{NoteName("C"), GChordType::Major, CMF::NoFlag};
    GCHECK("Note values major", chordC.noteValues(), noteValues({"C0", "E0", "G0"}));
    GCHECK("Short note name major", chordC.shortName(), String("C"));
    GCHECK("Long note name major", chordC.longName(), String("C Major"));
    GCHECK("Number notes", chordC.numberOfNotes(), Size{3});

    const GChord chordCopy{chordC};
    GCHECK("Copy constructor", chordCopy, chordC);

    GChord chordDm{NoteName("D"), GChordType::Minor, CMF::NoFlag};
    GCHECK("Note values minor", chordDm.noteValues(), noteValues({"D0", "F0", "A0"}));
    GCHECK("Short note name minor", chordDm.shortName(), String("Dm"));
    GCHECK("Long note name minor", chordDm.longName(), String("D minor"));

    GChord chordBdim{NoteName("B"), GChordType::Diminished, CMF::NoFlag};
    GCHECK("Note values diminished", chordBdim.noteValues(), noteValues({"B0", "D1", "F1"}));
    GCHECK("Short note name diminished", chordBdim.shortName(), String("B") + GNoteChar::Dim);
    GCHECK("Long note name diminished", chordBdim.longName(), String("B diminished"));

    GChord chordEaug{NoteName("E"), GChordType::Augmented, CMF::NoFlag};
    GCHECK("Note values augmented", chordEaug.noteValues(), noteValues({"E0", "Ab0", "C1"}));
    GCHECK("Short note name augmented", chordEaug.shortName(), String("E+"));
    GCHECK("Long note name augmented", chordEaug.longName(), String("E Augmented"));

    const GChord chordCmaj7{NoteName("C"), GChordType::Major, CMF::Major7};
    GCHECK("Note values Major 7", chordCmaj7.noteValues(), noteValues({"C0", "E0", "G0", "B"}));
    GCHECK("Short note name Major 7", chordCmaj7.shortName(), String("Cmaj7"));
    GCHECK("Long note name Major 7", chordCmaj7.longName(), String("C Major 7"));

    const GChord chordC7{NoteName("C"), GChordType::Major, CMF::Dominant7};
    GCHECK("Note values Dominant 7", chordC7.noteValues(), noteValues({"C0", "E0", "G0", "Bb"}));
    GCHECK("Short note name Dominant 7", chordC7.shortName(), String("C7"));
    GCHECK("Long note name Dominant 7", chordC7.longName(), String("C Dominant 7"));

    const GChord chordC9{NoteName("C"), GChordType::Major, CMF::Dominant7 | CMF::Dominant9};
    GCHECK("Note values Dominant 7 + Dominant 9", chordC9.noteValues(),
           noteValues({"C0", "E0", "G0", "Bb0", "D1"}));
    GCHECK("Short note name Dominant 7 + Dominant 9", chordC9.shortName(), String("C9"));
    GCHECK("Long note name Dominant 7 + Dominant 9", chordC9.longName(), String("C Dominant 9"));

    const GChord chordC7Add2{NoteName("C"), GChordType::Major, CMF::Dominant7 | CMF::Add2};
    GCHECK("Note values Dominant 7 + Add 2", chordC7Add2.noteValues(),
           noteValues({"C0", "D0", "E0", "G0", "Bb"}));
    GCHECK("Short note name Dominant 7 + Add 2", chordC7Add2.shortName(), String("C7+2"));
    GCHECK("Long note name Dominant 7 + Add 2", chordC7Add2.longName(), String("C Dominant 7 Add 2"));

    chordDm.cycleInversion();
    GCHECK("Cycle inversion 1", chordDm.noteValues(), noteValues({"A0", "D1", "F1"}));

    chordDm.cycleInversion();
    GCHECK("Cycle inversion 2", chordDm.noteValues(), noteValues({"F0", "A0", "D1"}));

    chordDm.cycleInversion();
    GCHECK("Cycle inversion 3", chordDm.noteValues(), noteValues({"D0", "F0", "A0"}));
}

} // namespace test