#include <iostream>

#include "g_logger.hpp"
#include "g_note_chars.hpp"
#include "g_scales.hpp"
#include "g_test_framework.hpp"

using namespace std;
using namespace gbase;
using namespace gmtheory;

namespace test {

GTEST(GScaleTest) {
    GLocalLogLevel localLogLevel(GLogger::LogLevel::None);

    const GScale scaleCmajor{NoteName("C"), GScaleType::NaturalMajor};
    const NoteValues scaleCmajorExpected{NoteNames{"C", "D", "E", "F", "G", "A", "B"} | noteNameToNoteValue};
    GCHECK("Note values C nataural major", scaleCmajor.noteValues(), scaleCmajorExpected);

    const NoteValues scaleCmajorExtendedExpected{NoteNames{"C1", "D1", "E1", "F1", "G1", "A1", "B1", "C2"} |
                                                 noteNameToNoteValue};
    GCHECK("Note values C1 nataural major extended", scaleCmajor.noteValues(noteValue("C1"), true),
           scaleCmajorExtendedExpected);

    const GScale scaleDdorian{NoteName("D"), GScaleType::Dorian};
    const NoteValues scaleDorianExpected{NoteNames{"D0", "E0", "F0", "G0", "A0", "B0", "C1"} |
                                         noteNameToNoteValue};
    GCHECK("Note values D dorian", scaleDdorian.noteValues(), scaleDorianExpected);

    const GScale scaleEphrygian{NoteName("E"), GScaleType::Phrygian};
    const NoteValues scalePhrygianExpected{NoteNames{"E0", "F0", "G0", "A0", "B0", "C1", "D1"} |
                                           noteNameToNoteValue};
    GCHECK("Note values E phrygian", scaleEphrygian.noteValues(), scalePhrygianExpected);

    const GScale scaleFlydian{NoteName("F"), GScaleType::Lydian};
    const NoteValues scaleLydianExpected{NoteNames{"F0", "G0", "A0", "B0", "C1", "D1", "E1"} |
                                         noteNameToNoteValue};
    GCHECK("Note values F lydian", scaleFlydian.noteValues(), scaleLydianExpected);

    const GScale scaleFmixolydian{NoteName("G"), GScaleType::Mixolydian};
    const NoteValues scaleMixolydianExpected{NoteNames{"G0", "A0", "B0", "C1", "D1", "E1", "F1"} |
                                             noteNameToNoteValue};
    GCHECK("Note values G mixolydian", scaleFmixolydian.noteValues(), scaleMixolydianExpected);

    const GScale scaleAminor{NoteName("A"), GScaleType::NaturalMinor};
    const NoteValues scaleAminorExpected{NoteNames{"A0", "B0", "C1", "D1", "E1", "F1", "G1"} |
                                         noteNameToNoteValue};
    GCHECK("Note values A natural minor", scaleAminor.noteValues(), scaleAminorExpected);

    const NoteValues scaleAminorExtendedExpected{NoteNames{"A0", "B0", "C1", "D1", "E1", "F1", "G1", "A1"} |
                                                 noteNameToNoteValue};
    GCHECK("Note values A natural minor extended", scaleAminor.noteValues(0, true),
           scaleAminorExtendedExpected);

    const GScale scaleBlocrian{NoteName("B"), GScaleType::Locrian};
    const NoteValues scaleLocrianExpected{NoteNames{"B0", "C1", "D1", "E1", "F1", "G1", "A1"} |
                                          noteNameToNoteValue};
    GCHECK("Note values B locrian", scaleBlocrian.noteValues(), scaleLocrianExpected);

    GCHECK("Scale name", scaleAminor.name(), String("A Natural minor"));

    GCHECK("Number of notes", scaleAminor.numberOfNotes(), Size{7});

    const NoteValues doBelongsoAminor{NoteNames{"A0", "B2", "C0", "F1", "A3"} | noteNameToNoteValue};
    const NoteValues dontBelongToAminor{NoteNames{"A0", "Bb2", "C0", "F1", "A3"} | noteNameToNoteValue};
    GCHECK("Belongs to scale 1", scaleAminor.contains(doBelongsoAminor), true);
    GCHECK("Belongs to scale 2", scaleAminor.contains(dontBelongToAminor), false);

    auto chordToChordShortName =
        std::ranges::views::transform([](const GChord &chord) -> String { return chord.shortName(); });

    const GVector<GChord> chordsOfCmajor{scaleCmajor.triadChords()};
    const GVector<String> expectedChordsOfCmajor{
        "C", "Dm", "Em", "F", "G", "Am", String("B") + GNoteChar::Dim};
    GCHECK("Chords of C Major", GVector<String>{chordsOfCmajor | chordToChordShortName},
           expectedChordsOfCmajor);

    const GVector<GChord> chordsOfAminor{scaleAminor.triadChords()};
    const GVector<String> expectedChordsOfAminor{"Am", String("B") + GNoteChar::Dim, "C", "Dm", "Em", "F",
                                                 "G"};
    GCHECK("Chords of A minor", GVector<String>{chordsOfAminor | chordToChordShortName},
           expectedChordsOfAminor);
}

} // namespace test