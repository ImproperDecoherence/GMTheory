#include "g_chord_modifiers.hpp"
#include "g_note_intervals.hpp"

namespace gmtheory {

//
const GDictionary<GModFlag, GChordModifier> ChordModifiers = {

    // clang-format off
    //                                   Long Name  Short Name  To Add, To Remove, To Cancel
    {CMF::NoFlag,       GChordModifier{ "",                 "", {}, {}, {}}},
    {CMF::Dominant7,    GChordModifier{ "Dominant 7",      "7", {NI::m7}, {}, {}}},
    {CMF::Major7,       GChordModifier{    "Major 7" ,  "maj7", {NI::M7}, {}, {}}},
    {CMF::Dominant9,    GChordModifier{ "Dominant 9",      "9", {NI::m7, NI::M9 }, {},  {CMF::Dominant7}}},
    {CMF::Dominant11,   GChordModifier{"Dominant 11",     "11", {NI::m7, NI::M9, NI::P11}, {}, {CMF::Dominant7, CMF::Dominant9}}},
    {CMF::Dominant13,   GChordModifier{"Dominant 13",     "13", {NI::m7, NI::M9, NI::P11, NI::M13}, {}, {CMF::Dominant7, CMF::Dominant9, CMF::Dominant11}}},
    {CMF::Suspended2nd, GChordModifier{"Suspended 2nd", "sus2", {NI::M2}, {NI::m3, NI::M3}, {}}},
    {CMF::Suspended4th, GChordModifier{"Suspended 4th", "sus4", {NI::P4}, {NI::m3, NI::M3}, {}}},
    {CMF::Add2,         GChordModifier{"Add 2",           "+2", {NI::M2}, {}, {}}},
    {CMF::Add6,         GChordModifier{"Add 6",           "+6", {NI::M6}, {}, {}}},
    {CMF::Add9,         GChordModifier{"Add 9",           "+9", {NI::M2}, {}, {}}},
    {CMF::Flat5th,      GChordModifier{"Flat 5th",        "b5", {NI::dim5}, {NI::P5}, {}}}
    // clang-format on
};

void GChordModifier::apply(GHarmony &harmony, NoteValue tonic) const {
    auto addTonic = std::ranges::views::transform([&](NoteValue value) { return value + tonic; });

    const auto toBeAdded = noteIntervalsToAdd_ | addTonic;
    const auto toBeRemoved = noteIntervalsToRemove_ | addTonic;

    GHarmony result{harmony};
    harmony += toBeAdded;
    harmony -= toBeRemoved;
}

void GChordModifier::appendLongName(String &input) const {

    // Fix to avoid getting "major major 7"
    if (input.contains(" Major") && !input.contains("Major 7")) {
        eraseSubString(input, " Major");
    }

    input.append(String(" ") + longName_);
}

GChordModFlags modFlagToChordModFlags(GModFlag flags) {
    auto ifFlagIsSet = std::ranges::views::filter([&](GModFlag mod) { return (flags & mod) > 0; });
    GChordModFlags result(ChordModifiers.keys() | ifFlagIsSet);
    return result;
}

GModFlag chordModFlagsToModFlag(const GChordModFlags flags) {
    GModFlag result = CMF::NoFlag;
    for (const auto &flag : flags) {
        result = result | flag;
    }
    return result;
}

} // namespace gmtheory