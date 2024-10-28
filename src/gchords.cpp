#include "gchords.hpp"
#include "gnotechars.hpp"
#include "gnoteintervals.hpp"
#include "gtypetools.hpp"

namespace gmtheory {

const GDictionary<GChordType, GChordTemplate> ChordTemplates = {
    // clang-format off
    {GChordType::Major,      GChordTemplate("Major",     "",              GHarmony{NI::R, NI::M3, NI::P5})},
    {GChordType::Minor,      GChordTemplate("minor",     "m",             GHarmony{NI::R, NI::m3, NI::P5})},
    {GChordType::Diminished, GChordTemplate("diminished", GNoteChar::Dim, GHarmony{NI::R, NI::m3, NI::dim5})},
    {GChordType::Augmented,  GChordTemplate("Augmented", "+",             GHarmony{NI::R, NI::M3, NI::Aug5})}
    // clang-format on
};

void GChord::setFlags(const GChordModFlags &flags) {
    GChordModFlags oldFlags = modFlags_;
    GChordModFlags tempFlags = flags;
    tempFlags.erase(CMF::NoFlag);
    modFlags_ = flags;

    for (auto flag : tempFlags) {
        modFlags_ -= ChordModifiers[flag].cancelsModifiers();
    }

    if (modFlags_ != oldFlags) {
        notifications.chordChanged(*this);
    }
}

NoteValues GChord::noteValues() const {
    GHarmony noteValues{template_.noteValues(tonic_)};

    // Apply chord modifications
    for (const auto &flag : modFlags_) {
        ChordModifiers[flag].apply(noteValues, tonic_);
    }

    // Apply inversion
    NoteValues result{noteValues};
    for (Inversion i = 0; i < inversion_; ++i) {
        result.rotate(1);
        result[0] -= NI::Octave;
        if (result[0] < 0) {
            result = NoteValues{result | transpose(NI::Octave)};
        }
    }

    return result;
}

void GChord::setInversion(Inversion inversion) {
    if (inversion != inversion_) {
        inversion_ = inversion % numberOfNotes();
        notifications.chordChanged(*this);
    }
}

void GChord::setTonic(NoteValue noteValue) {
    if (noteValue != tonic_) {
        tonic_ = noteValue;
        notifications.chordChanged(*this);
    }
}

String GChord::shortModName(NoteNameStyle style) const {
    String modName;

    for (auto flag : modFlags_) {
        ChordModifiers[flag].appendShortName(modName);
    }

    const auto baseNoteValue = noteValues()[0];
    if (baseNoteValue != tonic_) {
        const auto showOctave = false;
        modName.append(String("/") + noteName(baseNoteValue, style, showOctave));
    }

    return modName;
};

String GChord::longName(NoteNameStyle style) const {
    String name = template_.longName(tonic_, style);

    for (auto flag : modFlags_) {
        ChordModifiers[flag].appendLongName(name);
    }

    return name;
}

void GChord::print(std::ostream &target) const {
    target << "GChord" << "{ ";
    target << this->shortName() << " | " << NoteNames{noteValues() | noteValueToNoteName()};
    target << " }";
}

} // namespace gmtheory