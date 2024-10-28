/**
 * @file gnoteintervals.hpp
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

#include "gbasictypes.hpp"
#include "gdictionary.hpp"
#include "genumerate.hpp"
#include "gvector.hpp"

namespace gmtheory {

/**
 * @brief Notes are represented by note values which can have values from 0 and ascending. Value 0 corresponds
 * to the note C0, value 1 corresponds to the note Db0, etc.
 */
using NoteValue = gbase::Integer;

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
constexpr NoteValue PerfectUnison = 0;
constexpr NoteValue Root = 0;
constexpr NoteValue Tonic = 0;
constexpr NoteValue SemiTone = 1;
constexpr NoteValue Tone = 2 * SemiTone;
constexpr NoteValue minor2nd = 1 * SemiTone;
constexpr NoteValue Major2nd = 2 * SemiTone;
constexpr NoteValue minor3rd = 3 * SemiTone;
constexpr NoteValue Major3rd = 4 * SemiTone;
constexpr NoteValue Perfect4th = 5 * SemiTone;
constexpr NoteValue Diminished5th = 6 * SemiTone;
constexpr NoteValue Augmented4th = 6 * SemiTone;
constexpr NoteValue TriTone = 6 * SemiTone;
constexpr NoteValue Perferct5th = 7 * SemiTone;
constexpr NoteValue Augmented5th = 8 * SemiTone;
constexpr NoteValue minor6th = 8 * SemiTone;
constexpr NoteValue Major6th = 9 * SemiTone;
constexpr NoteValue minor7th = 10 * SemiTone;
constexpr NoteValue Major7th = 11 * SemiTone;
constexpr NoteValue PerfectOctave = 12 * SemiTone;
constexpr NoteValue minor9th = 13 * SemiTone;
constexpr NoteValue Major9th = 14 * SemiTone;
constexpr NoteValue Perfect11th = 17 * SemiTone;
constexpr NoteValue Major13th = 21 * SemiTone;
constexpr NoteValue Octave = PerfectOctave;

// Abbrevations
constexpr NoteValue P1 = PerfectUnison;
constexpr NoteValue R = Root;
constexpr NoteValue m2 = minor2nd;
constexpr NoteValue M2 = Major2nd;
constexpr NoteValue m3 = minor3rd;
constexpr NoteValue M3 = Major3rd;
constexpr NoteValue P4 = Perfect4th;
constexpr NoteValue dim5 = Diminished5th;
constexpr NoteValue Aug4 = Augmented4th;
constexpr NoteValue T = TriTone;
constexpr NoteValue P5 = Perferct5th;
constexpr NoteValue Aug5 = Augmented5th;
constexpr NoteValue m6 = minor6th;
constexpr NoteValue M6 = Major6th;
constexpr NoteValue m7 = minor7th;
constexpr NoteValue M7 = Major7th;
constexpr NoteValue P8 = PerfectOctave;
constexpr NoteValue m9 = minor9th;
constexpr NoteValue M9 = Major9th;
constexpr NoteValue P11 = Perfect11th;
constexpr NoteValue M13 = Major13th;
constexpr NoteValue O = Octave;
} // namespace NI

/**
 * @brief Dictionary containing the short names for different note intervals, e.g "m3" for Major3rd.
 */
extern const gbase::GDictionary<NoteValue, NoteName> NoteIntervalShortNames;

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