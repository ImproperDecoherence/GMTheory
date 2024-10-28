#include "gnotes.hpp"
#include "gbasictypes.hpp"
#include "testframework.hpp"

namespace test {

using namespace gmtheory;
using namespace gbase;

GTEST(GNotesTest) {
    GCHECK("Note name style 1", noteNameStyle(noteNamesSharpTemplate), NoteNameStyle::sharp);
    GCHECK("Note name style 2", noteNameStyle(noteNamesFlatTemplate), NoteNameStyle::flat);

    GCHECK("Note name flat", noteName(1, NoteNameStyle::flat), String("Db0"));
    GCHECK("Note name sharp", noteName(1, NoteNameStyle::sharp), String("C#0"));
    GCHECK("Note name sharp", noteName(1, NoteNameStyle::sharp, false), String("C#"));

    GCHECK("Note value", noteValue(NoteName("Db1")), NI::Octave + 1);

    GCHECK("Note to value 1", noteToNoteValue(1), 1);
    GCHECK("Note to value 1", noteToNoteValue(NoteName("Db")), 1);

    NoteNames generatedNoteNames;
    const NoteNames expextedGeneratedNoteNames = {"C0",  "Db0", "D0",  "Eb0", "E0",  "F0",
                                                  "Gb0", "G0",  "Ab0", "A0",  "Bb0", "B0"};
    const bool showOctave{true};
    for (auto name : noteNames(0, NI::Octave, NoteNameStyle::flat, showOctave)) {
        generatedNoteNames.emplaceBack(name);
    }
    GCHECK("Generated note names", generatedNoteNames, expextedGeneratedNoteNames);

    const NoteNames unorderedNoteNames = {"Ab0", "Db0", "Bb1"};
    const NoteNames expectedSortedNoteNames = {"Db0", "Ab0", "Bb1"};
    const NoteNames sortedNoteNames = sortNoteNames(unorderedNoteNames);
    GCHECK("Sort notnames", sortedNoteNames, expectedSortedNoteNames);

    const NoteNames noteNames = {"Db0", "Ab0", "Bb1"};
    const NoteValues expectedNoteValues = {1, 8, NI::Octave + 10};
    const NoteValues noteValues{noteNames | noteNameToNoteValue};
    GCHECK("Note names to note values", noteValues, expectedNoteValues);

    const NoteNames valuesToNames{noteValues | noteValueToNoteName(NoteNameStyle::flat, showOctave)};
    GCHECK("Note value to note name", valuesToNames, noteNames);

    const NoteNames stripped{noteNames | stripOctaveFromNoteName};
    const NoteNames expectedStrippedNoteNames = {"Db", "Ab", "Bb"};
    GCHECK("Strip octave", stripped, expectedStrippedNoteNames);

    const NoteName diatonicNoteName{"A"};
    const NoteName nonDiatonicNoteName1{"Ab"};
    const NoteName nonDiatonicNoteName2{"C#"};
    GCHECK("Is diatonic 1", isDiatonic(diatonicNoteName), true);
    GCHECK("Is diatonic 2", isDiatonic(nonDiatonicNoteName1), false);
    GCHECK("Is diatonic 3", isDiatonic(nonDiatonicNoteName2), false);

    const NoteNames toBeRebasedNoteNames = {"Eb1", "F2", "G1", "A2"};
    const NoteValues toBeRebasedNoteValues{toBeRebasedNoteNames | noteNameToNoteValue};
    const NoteValue currentBaseNoteValue = baseNoteValue(toBeRebasedNoteValues);
    GCHECK("Base note value", currentBaseNoteValue, noteValue(NoteName("C1")));

    const NoteValue targetBaseNoteValue{noteValue(NoteName{"C3"})};
    const NoteNames expectedRebasedNoteNames = {"Eb3", "F4", "G3", "A4"};
    const NoteValues expectedRebasedNoteValues{expectedRebasedNoteNames | noteNameToNoteValue};
    const NoteValues rebasedNoteValues1{toBeRebasedNoteValues |
                                        rebaseNoteValue(targetBaseNoteValue, currentBaseNoteValue)};
    GCHECK("Rebase notes 1", rebasedNoteValues1, expectedRebasedNoteValues);

    const NoteValues rebasedNoteValues2{rebaseNoteValues(toBeRebasedNoteValues, targetBaseNoteValue)};
    GCHECK("Rebase notes 2", rebasedNoteValues2, expectedRebasedNoteValues);

    const NoteNames rebaseSequenceNoteNames1 = {"Eb1", "F1", "G1", "A1"};
    const NoteNames rebaseSequenceNoteNames2 = {"Eb2", "F2", "G2", "A2"};
    const NoteNames rebaseSequenceNoteNames3 = {"Eb3", "F3", "G3", "A3"};
    const NoteValues rebaseSequenceNoteValues1{rebaseSequenceNoteNames1 | noteNameToNoteValue};
    const NoteValues rebaseSequenceNoteValues2{rebaseSequenceNoteNames2 | noteNameToNoteValue};
    const NoteValues rebaseSequenceNoteValues3{rebaseSequenceNoteNames3 | noteNameToNoteValue};
    const GVector<NoteValues> toRebaseHarmonySequence = {rebaseSequenceNoteValues1, rebaseSequenceNoteValues2,
                                                         rebaseSequenceNoteValues3};

    const NoteNames expectedRebaseSequenceNoteNames1 = {"Eb3", "F3", "G3", "A3"};
    const NoteNames expectedRebaseSequenceNoteNames2 = {"Eb4", "F4", "G4", "A4"};
    const NoteNames expectedRebaseSequenceNoteNames3 = {"Eb5", "F5", "G5", "A5"};
    const NoteValues expectedRebaseSequenceNoteValues1{expectedRebaseSequenceNoteNames1 |
                                                       noteNameToNoteValue};
    const NoteValues expectedRebaseSequenceNoteValues2{expectedRebaseSequenceNoteNames2 |
                                                       noteNameToNoteValue};
    const NoteValues expectedRebaseSequenceNoteValues3{expectedRebaseSequenceNoteNames3 |
                                                       noteNameToNoteValue};
    const GVector<NoteValues> expecedRebasedHarmonySequence = {expectedRebaseSequenceNoteValues1,
                                                               expectedRebaseSequenceNoteValues2,
                                                               expectedRebaseSequenceNoteValues3};

    GCHECK("Rebase sequence", rebaseNoteValues(toRebaseHarmonySequence, targetBaseNoteValue),
           expecedRebasedHarmonySequence);

    NoteValues i0{};
    const auto s0 = signature(i0);
    GCHECK("Signature 0", s0, Signature{0});

    NoteValues i1{0};
    const auto s1 = signature(i1);
    GCHECK("Signature 1", s1, Signature{1});

    NoteValues i2{0, 1};
    const auto s2 = signature(i2);
    GCHECK("Signature 2", s2, Signature{3});

    NoteValues i3{0, 1, 2};
    const auto s3 = signature(i3);
    GCHECK("Signature 3", s3, Signature{7});

    const auto ns = nearSignatures(s3, 1);
    const GSet<Signature> e4 = {6, 5, 3, 15, 23, 39, 71, 135, 263, 519, 1031, 2055};
    GCHECK("Near signature", ns, e4);
}

} // namespace test