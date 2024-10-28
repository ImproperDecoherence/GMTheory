#include <numeric>

#include "gchords.hpp"
#include "genumerate.hpp"
#include "gexceptions.hpp"
#include "glogger.hpp"
#include "gscaledegrees.hpp"
#include "gscales.hpp"

namespace gmtheory {

const GScaleIntervals NaturalMajorIntervals{NI::R, NI::M2, NI::M3, NI::P4, NI::P5, NI::M6, NI::M7, NI::O};
const GScaleIntervals HarmonicMinorIntervals{NI::R, NI::M2, NI::m3, NI::P4, NI::P5, NI::m6, NI::M7, NI::O};

const GDictionary<GScaleType, GScaleTemplate> ScaleTemplates = {
    // clang-format off
    {GScaleType::Lydian,        GScaleTemplate{.intervals = NaturalMajorIntervals,  .mode = 4, .name = "Lydian"}},
    {GScaleType::NaturalMajor,  GScaleTemplate{.intervals = NaturalMajorIntervals,  .mode = 1, .name = "Natural Major"}},
    {GScaleType::Mixolydian,    GScaleTemplate{.intervals = NaturalMajorIntervals,  .mode = 5, .name = "Mixolydian"}},
    {GScaleType::Dorian,        GScaleTemplate{.intervals = NaturalMajorIntervals,  .mode = 2, .name = "Dorian"}},
    {GScaleType::NaturalMinor,  GScaleTemplate{.intervals = NaturalMajorIntervals,  .mode = 6, .name = "Natural minor"}},
    {GScaleType::Phrygian,      GScaleTemplate{.intervals = NaturalMajorIntervals,  .mode = 3, .name = "Phrygian"}},
    {GScaleType::Locrian,       GScaleTemplate{.intervals = NaturalMajorIntervals,  .mode = 7, .name = "Locrian"}},

    {GScaleType::HarmonicMinor,    GScaleTemplate{.intervals = HarmonicMinorIntervals, .mode = 1, .name = "Harmonic minor"}},
    {GScaleType::LocrianNatural6,  GScaleTemplate{.intervals = HarmonicMinorIntervals, .mode = 2, .name = "Locrian Natural 6"}},
    {GScaleType::AugmentedMajor,   GScaleTemplate{.intervals = HarmonicMinorIntervals, .mode = 3, .name = "Augmented Major"}},
    {GScaleType::UkranianDorian,   GScaleTemplate{.intervals = HarmonicMinorIntervals, .mode = 4, .name = "Ukranian Dorian"}},
    {GScaleType::PhrygianDominant, GScaleTemplate{.intervals = HarmonicMinorIntervals, .mode = 5, .name = "Phrygian Dominant"}},
    {GScaleType::LydianNatural2,   GScaleTemplate{.intervals = HarmonicMinorIntervals, .mode = 6, .name = "Lydian Natural 2"}},
    {GScaleType::SuperLocrian,     GScaleTemplate{.intervals = HarmonicMinorIntervals, .mode = 7, .name = "Super Locrian"}}
    // clang-format on
};

NoteValues GScale::noteValues(const NoteValue baseNoteValue, const bool includePerfectOctave) const {
    const GScaleIntervals &templateIntervals = template_.intervals;

    if (baseNoteValue % NI::Octave != 0) {
        GTHROW(GInvalidArgument, "baseNoteValue must be a C note: ", baseNoteValue);
    }

    // Calculate step sizes between notes for template intervals.
    NoteValues steps(templateIntervals.size() - 1);
    auto diff = [](NoteValue a, NoteValue b) { return a - b; };
    std::transform(templateIntervals.begin() + 1, templateIntervals.end(), templateIntervals.begin(),
                   steps.begin(), diff);

    // Calculate step sizes between notes for template mode.
    steps.rotate(1 - template_.mode);
    if (includePerfectOctave) {
        steps += steps[0];
    }

    // Calculate intervals between tonic and the notes of the scale.
    NoteValues modeIntervals(steps.size());
    std::exclusive_scan(steps.begin(), steps.end(), modeIntervals.begin(), 0);

    // Transpose to the key of the scale.
    return NoteValues(modeIntervals | transpose(tonic_ + baseNoteValue));
}

GVector<GChord> GScale::triadChords() const {
    GVector<GChord> results;
    const NoteValues scaleNoteValues = noteValues() + NoteValues{noteValues() | transpose(NI::Octave)};

    for (const auto i : std::views::iota(Size{0}, numberOfNotes())) {
        const NoteValues chordNoteValues = {scaleNoteValues[i + GScaleDegree::Tonic],
                                            scaleNoteValues[i + GScaleDegree::Mediant],
                                            scaleNoteValues[i + GScaleDegree::Dominant]};
        const NoteValue tonic{chordNoteValues[0]};
        const GHarmony chordIntervals{chordNoteValues | transpose(-tonic)};

        for (const auto &entry : ChordTemplates) {
            const auto &chordType = entry.first;
            const auto &chordTemplate = entry.second;

            if (chordTemplate.intervals.isSubsetOf(chordIntervals)) {
                results.emplaceBack(GChord(tonic, chordType, CMF::NoFlag));
            }
        }
    }

    return results;
}

} // namespace gmtheory