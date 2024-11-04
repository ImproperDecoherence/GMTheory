/**
 * @file g_note_intervals.hpp
 * @author ImproperDecoherence (gustowny@gmail.com)
 * @brief Defines the most basic relations of music theory.
 * @version 0.1
 * @date 2024-10-27
 *
 * Copyright (c) 2024-2024 Jonas Gustavsson
 *
 */

#pragma once

#include <map>
#include <ranges>
#include <string>
#include <vector>

#include "g_basic_types.hpp"
#include "g_dictionary.hpp"
#include "g_enumerate.hpp"
#include "g_vector.hpp"

namespace gmtheory {

/**
 * @brief Notes are represented by note values which can have values from 0 and ascending. Value 0 corresponds
 * to the note C0, value 1 corresponds to the note Db0, etc.
 */
using NoteValue = gbase::Integer;

/**
 * @brief Note intervals are represented as number of semitone steps.
 */
using NoteInterval = gbase::Integer;

/**
 * @brief Note names are represented by strings, e.g. 'C', 'Db', etc. The octave of a note can be indicated as
 * a postfix, e.g. 'C0', 'C1', etc.
 */
using NoteName = gbase::String;

using Octaves = gbase::Integer;
using NoteValues = gbase::GVector<NoteValue>;
using NoteNames = gbase::GVector<NoteName>;

/**
 * @brief Defines names and values for basic note intervals.
 */
namespace NI {

// Tone interval definitions
constexpr NoteInterval PerfectUnison = 0;
constexpr NoteInterval Root = 0;
constexpr NoteInterval Tonic = 0;
constexpr NoteInterval SemiTone = 1;
constexpr NoteInterval Tone = 2 * SemiTone;
constexpr NoteInterval minor2nd = 1 * SemiTone;
constexpr NoteInterval Major2nd = 2 * SemiTone;
constexpr NoteInterval minor3rd = 3 * SemiTone;
constexpr NoteInterval Major3rd = 4 * SemiTone;
constexpr NoteInterval Perfect4th = 5 * SemiTone;
constexpr NoteInterval Diminished5th = 6 * SemiTone;
constexpr NoteInterval Augmented4th = 6 * SemiTone;
constexpr NoteInterval TriTone = 6 * SemiTone;
constexpr NoteInterval Perferct5th = 7 * SemiTone;
constexpr NoteInterval Augmented5th = 8 * SemiTone;
constexpr NoteInterval minor6th = 8 * SemiTone;
constexpr NoteInterval Major6th = 9 * SemiTone;
constexpr NoteInterval minor7th = 10 * SemiTone;
constexpr NoteInterval Major7th = 11 * SemiTone;
constexpr NoteInterval PerfectOctave = 12 * SemiTone;
constexpr NoteInterval minor9th = 13 * SemiTone;
constexpr NoteInterval Major9th = 14 * SemiTone;
constexpr NoteInterval Perfect11th = 17 * SemiTone;
constexpr NoteInterval Major13th = 21 * SemiTone;
constexpr NoteInterval Octave = PerfectOctave;

// Abbrevations
constexpr NoteInterval P1 = PerfectUnison;
constexpr NoteInterval R = Root;
constexpr NoteInterval m2 = minor2nd;
constexpr NoteInterval M2 = Major2nd;
constexpr NoteInterval m3 = minor3rd;
constexpr NoteInterval M3 = Major3rd;
constexpr NoteInterval P4 = Perfect4th;
constexpr NoteInterval dim5 = Diminished5th;
constexpr NoteInterval Aug4 = Augmented4th;
constexpr NoteInterval T = TriTone;
constexpr NoteInterval P5 = Perferct5th;
constexpr NoteInterval Aug5 = Augmented5th;
constexpr NoteInterval m6 = minor6th;
constexpr NoteInterval M6 = Major6th;
constexpr NoteInterval m7 = minor7th;
constexpr NoteInterval M7 = Major7th;
constexpr NoteInterval P8 = PerfectOctave;
constexpr NoteInterval m9 = minor9th;
constexpr NoteInterval M9 = Major9th;
constexpr NoteInterval P11 = Perfect11th;
constexpr NoteInterval M13 = Major13th;
constexpr NoteInterval O = Octave;
} // namespace NI

/**
 * @brief Dictionary containing the short names for different note intervals, e.g "m3" for Major3rd.
 */
extern const gbase::GDictionary<NoteInterval, NoteName> NoteIntervalShortNames;

/**
 * @brief Transform to be used with ranges to normalize a range of note values to values within octave 0.
 *
 * Example usage:
 * @code {.language-id}
 * const NoteValues notes{1, 17, 25};
 * const NoteValues normalizedNotes{notes | normalize};
 * @endcode
 */
constexpr auto normalize = std::ranges::views::transform([](NoteValue value) { return value % NI::Octave; });

/**
 * @brief Transform to be used with ranges of note values to transpose the note value.
 *
 * @param term The number of semi tones to transpose the note value.
 *
 * Example usage:
 * @code {.language-id}
 * const NoteValues notes{1, 17, 25};
 * const NoteValues transposedNotes{notes | transpose(NI::Octave)};
 * @endcode
 */
constexpr auto transpose(NoteValue term) {
    auto fn = std::ranges::views::transform([=](NoteValue value) { return value + term; });
    return fn;
}

/**
 * @brief Transform which transposes a range of note values a given number of octaves.
 *
 * @param octaves The number of octaves to transpose the notes.
 */
constexpr auto octaveTransposition(NoteValue octaves) {
    auto fn = std::ranges::views::transform([=](NoteValue value) { return value + octaves * NI::Octave; });
    return fn;
}

/**
 * @brief Returns the interval short name for an interval, e.g. 'm3' for NI::Major3rd.
 *
 * Throws an exception if the name for an interval is not defined.
 */
const NoteName &noteIntervalShortName(NoteValue interval);

} // namespace gmtheory