/**
 * @file g_scales.hpp
 * @author ImproperDecoherence (gustowny@gmail.com)
 * @brief Defines the GScale class and available scales.
 * @version 0.1
 * @date 2024-10-28
 *
 * Copyright (c) 2024-2024 Jonas Gustavsson
 *
 */

#pragma once

#include <algorithm>

#include "g_chords.hpp"
#include "g_harmony.hpp"
#include "g_note_intervals.hpp"
#include "g_notes.hpp"
#include "g_vector.hpp"

using namespace gbase;

namespace gmtheory {

/**
 * @brief Type used for a range of note intervals which defines a scale.
 */
using GScaleIntervals = GVector<NoteValue>;

/**
 * @brief Type used for defining the mode of a scale.
 */
using GMode = Integer;

/**
 * @brief Implementation of the natual C Major scale.
 */
extern const GScaleIntervals NaturalMajorIntervals;

/**
 * @brief Implementation of the harminic C minor scale.
 */
extern const GScaleIntervals HarmonicMinorIntervals;

/**
 * @brief A scale is defined by its note intervals and its mode.
 */
struct GScaleTemplate {
    const GScaleIntervals &intervals;
    const GMode mode{0};
    const String name;
};

/**
 * @brief Identifiers for available scales.
 */
enum class GScaleType {
    // Modes of natural Major
    Lydian,
    NaturalMajor,
    Mixolydian,
    Dorian,
    NaturalMinor,
    Phrygian,
    Locrian,

    // Modes of harmonic minor
    HarmonicMinor,
    LocrianNatural6,
    AugmentedMajor,
    UkranianDorian,
    PhrygianDominant,
    LydianNatural2,
    SuperLocrian
};

/**
 * @brief Implementaion of available scales.
 */
extern const GDictionary<GScaleType, GScaleTemplate> ScaleTemplates;

/**
 * @brief Represents a musical scale.
 */
class GScale {
  public:
    /**
     * @brief Constructs a new GScale object.
     *
     * @tparam Note A gmtheory::NoteValue or gmtheory::NoteName.
     * @param tonic The tonic note value OR note name of the scale.
     * @param scaleType Scale type identifier.
     *
     * Example usage:
     * @code
     * const GScale scaleDdorian{NoteName("D"), GScaleType::Dorian};
     * @endcode
     *
     */
    template <typename Note>
    GScale(Note tonic, GScaleType scaleType)
        : template_{&(ScaleTemplates.at(scaleType))}, tonic_{noteToNoteValue(tonic)} {}

    GScale() = delete;
    GScale &operator=(const GScale &) = default;
    GScale(const GScale &) = default;

    bool operator==(const GScale &) const = default;
    bool operator!=(const GScale &) const = default;

    /**
     * @brief Returns the note values of the scale.
     *
     * @param baseNoteValue The base note value is a C-note value which is closest to the note values of the
     * scale but also lower than all notes of the scale.
     * @param includePerfectOctave Defines if the perfect octave shall be included (i.e. the C1 note in a
     * C-scale starting at C0).
     */
    GHarmony noteValues(NoteValue baseNoteValue = 0, bool includePerfectOctave = false) const;

    /**
     * @brief Returns the tonic note value of the scale.
     */
    constexpr NoteValue tonicNoteValue() const { return tonic_; }

    /**
     * @brief Returns the name of the scale, e.g. 'Natural Major'.
     */
    constexpr String scaleName() const { return template_->name; }

    /**
     * @brief Returns the full name of the scale, including the tonic, e.g. 'C Natural Major'.
     *
     * @param style NoteNameStyle::flat or NoteNameStyle::sharp.
     */
    constexpr String name(NoteNameStyle style = NoteNameStyle::flat) const {
        return String(noteName(tonic_, style, false)) + " " + scaleName();
    }

    /**
     * @brief Checks if a given range of notes belongs to the scale.
     */
    template <RangeOfNoteValues Range> bool contains(const Range &noteValueRange) const {
        const GHarmony normalizedScaleValues{noteValues() | normalize};
        const GHarmony normalizedNoteValues{noteValueRange | normalize};
        return normalizedNoteValues.isSubsetOf(normalizedScaleValues);
    }

    /**
     * @brief Checks if a given note belongs to the scale.
     */
    constexpr bool contains(NoteValue noteValue) const { return contains(GHarmony{noteValue}); }

    /**
     * @brief Returns the number of notes of the scale.
     */
    constexpr Size numberOfNotes() const { return static_cast<Size>(template_->intervals.size() - 1); }

    /**
     * @brief Returns the triad chords of the scale.
     */
    GVector<GChord> triadChords() const;

    NoteName noteNameInScale(NoteValue) const;

    /**
     * @brief Prints a textual representsion of the scale state.
     */
    void print(std::ostream &target) const;

  private:
    const GScaleTemplate *template_{nullptr};
    NoteValue tonic_{0};
};

constexpr std::ostream &operator<<(std::ostream &s, const GScale &scale) {
    scale.print(s);
    return s;
}

} // namespace gmtheory