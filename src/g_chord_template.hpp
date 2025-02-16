/**
 * @file g_chord_template.hpp
 * @author ImproperDecoherence (gustowny@gmail.com)
 * @brief Defines the class GChordTemplate
 * @version 0.1
 * @date 2024-10-28
 *
 * Copyright (c) 2024-2024 Jonas Gustavsson
 *
 */

#include "g_basic_types.hpp"
#include "g_harmony.hpp"
#include "g_notes.hpp"

#pragma once

namespace gmtheory {

/**
 * @brief A template from which chords can be created. A chord template is defined by note interval pattern.
 */
class GChordTemplate {
  public:
    GChordTemplate() = delete;
    ~GChordTemplate() = default;

    bool operator==(const GChordTemplate &) const = default;
    bool operator!=(const GChordTemplate &) const = default;
    auto operator<=>(const GChordTemplate &) const = default;

    /**
     * @brief Constructs a new GChordTemplate object.
     *
     * @param longName The long name of the chord type, e.g. 'minor'.
     * @param shortName The short name of the chord type, e.g. 'm'.
     * @param interv The note intervals which defines the chord type. The first note value must be 0, i.e. the
     * intervals shall represent the C-version of the chord type.
     */
    GChordTemplate(const String &longName, const String &shortName, const GHarmony &interv)
        : longName_{longName}, shortName_{shortName}, intervals{interv} {}

    /**
     * @brief Returns the note values of the chord type.
     *
     * @param tonic The tonic note value of the chord, e.g. 0 for a C-chord and 2 for a D-chord.
     */
    constexpr auto noteValues(NoteValue tonic) const { return intervals | transpose(tonic); }

    /**
     * @brief Returns the short name of the chord type.
     *
     * @param tonic The tonic note value of the chord, e.g. 0 for a C-chord and 2 for a D-chord.
     * @param style The style of the note names ('sharp' or 'flat').
     */
    constexpr String shortName(NoteValue tonic, NoteNameStyle style = DefaultNoteNameStyle) const {
        const bool showOctave = false;
        return noteName(tonic, style, showOctave) + shortName_;
    }

    constexpr String postFix() const { return shortName_; }

    /**
     * @brief Returns the long name of the chord type.
     *
     * @param tonic The tonic note value of the chord, e.g. 0 for a C-chord and 2 for a D-chord.
     * @param style The style of the note names ('sharp' or 'flat').
     */
    constexpr String longName(NoteValue tonic, NoteNameStyle style = DefaultNoteNameStyle) const {
        const bool showOctave = false;
        return noteName(tonic, style, showOctave) + " " + longName_;
    }

  private:
    String longName_;
    String shortName_;

  public:
    const GHarmony intervals;
};

} // namespace gmtheory