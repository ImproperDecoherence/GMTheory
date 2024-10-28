#include <regex>

#include "galgorithms.hpp"
#include "gexceptions.hpp"
#include "gnoteintervals.hpp"
#include "gnotes.hpp"

namespace gmtheory {

GSet<Signature> nearSignatures(Signature signature, Integer distance) {
    GSet<Signature> result;

    if (distance < 0) {
        GTHROW(GInvalidArgument, "Distance must be positive: ", distance);
    }

    if (distance == 0) {
        result.extend(signature);
        return result;
    }

    const auto bitsToToggle = combinations(std::views::iota(0, NI::Octave), distance);

    for (auto bits : bitsToToggle) {
        Unsigned mask = 0;

        for (auto bit : bits) {
            mask = mask | (1 << bit);
        }

        result.extend(signature ^ mask); // xor toggles the bits in the mask
    }

    return result;
}

NoteName noteName(NoteValue noteValue, NoteNameStyle style, bool showOctave) {

    auto noteNames = noteNamesSharpTemplate;
    if (style == NoteNameStyle::flat) {
        noteNames = noteNamesFlatTemplate;
    }

    String postFix;
    if (showOctave) {
        postFix = std::to_string(noteValue / NI::Octave);
    }

    return noteNames[noteValue % NI::Octave] + postFix;
}

NoteValue noteValue(const NoteName &noteName) {
    const auto style = noteNameStyle(noteName);

    auto *pNoteNames = &noteNamesSharpTemplate;
    if (style == NoteNameStyle::flat) {
        pNoteNames = &noteNamesFlatTemplate;
    }

    const std::regex pattern(R"((\D+)(\d*))");
    std::smatch match;
    Octaves octave = 0;
    NoteValue baseNoteValue = 0;

    if (std::regex_search(noteName, match, pattern)) {
        const String baseNoteName = match[1]; // Base note name
        const String octaveStr = match[2];    // Octave string

        if (!octaveStr.empty()) {
            octave = std::stoi(octaveStr);
        }

        const auto index = pNoteNames->find(baseNoteName);

        if (index >= 0) {
            baseNoteValue = index;
        } else {
            GTHROW(GInvalidArgument, "Argument must be a valid note name: ", noteName);
        }

    } else {
        GTHROW(GInvalidArgument, "Argument must be a valid note name: ", noteName);
    }

    return baseNoteValue + octave * NI::Octave;
}

} // namespace gmtheory