/**
 * @file gnotes.hpp
 * @author ImproperDecoherence (gustowny@gmail.com)
 * @brief Defines note names and basic operations on note values and note names.
 * @version 0.1
 * @date 2024-10-27
 *
 * Copyright (c) 2024-2024 Jonas Gustavsson
 *
 */

#pragma once

#include <algorithm>
#include <cctype>
#include <coroutine>
#include <ranges>
#include <string>

#include "gexceptions.hpp"
#include "glogger.hpp"
#include "gnoteintervals.hpp"
#include "gset.hpp"
#include "gtypetools.hpp"
#include "gvector.hpp"

namespace gmtheory {

using namespace gbase;

namespace BasicNoteValue {
constexpr NoteValue C = 0;
constexpr NoteValue Db = 1;
constexpr NoteValue D = 2;
constexpr NoteValue Eb = 3;
constexpr NoteValue E = 4;
constexpr NoteValue F = 5;
constexpr NoteValue Gb = 6;
constexpr NoteValue G = 7;
constexpr NoteValue Ab = 8;
constexpr NoteValue A = 9;
constexpr NoteValue Bb = 10;
constexpr NoteValue B = 11;
}; // namespace BasicNoteValue

/**
 * @brief The Signature type represents an unique value for all possible normalized harmonies.
 *
 */
using Signature = Unsigned;

/**
 * @brief Template for note names which uses the 'sharp' notation.
 */
const GVector<NoteName> noteNamesSharpTemplate = {"C",  "C#", "D",  "D#", "E",  "F",
                                                  "F#", "G",  "G#", "A",  "A#", "B"};

/**
 * @brief Template for note names which uses the 'flat' notation.
 */
const GVector<NoteName> noteNamesFlatTemplate = {"C",  "Db", "D",  "Eb", "E",  "F",
                                                 "Gb", "G",  "Ab", "A",  "Bb", "B"};

/**
 * @brief Concept for a range of note values.
 */
template <typename Range>
concept RangeOfNoteValues = RangeOf<Range, NoteValue>;

/**
 * @brief Concept for a range of note names.
 */
template <typename Range>
concept RangeOfNoteNames = RangeOf<Range, NoteName>;

/**
 * @brief Concept for a range of range of note values.
 */
template <typename Range>
concept RangeOfRangeOfNoteValues = RangeOfRange<Range> && IsRangeOfRangeOf<Range, NoteValue>;

/**
 * @brief Calculates the Signature (see doc for the Signature type) for a given range of note values.
 */
template <RangeOfNoteValues Range> constexpr Signature signature(const Range &intervals) {
    constexpr Signature mask{1};
    Unsigned signature{0};

    for (const auto noteValue : intervals | normalize) {
        auto value = static_cast<Unsigned>(noteValue);
        signature = signature | (mask << value);
    }

    return static_cast<Signature>(signature);
}

/**
 * @brief Returns a set of Signatures which are close to a given seed Signature.
 *
 * @param signature The seed Signature.
 * @param distance The distance is the number of notes that differs in the harmony that the Signature
 * represents. E.g. distance=0 will only return the seed Signature, distance=1 with a seed that corresponds to
 * the C chord will return Signatures for C7, Cmaj7, etc.
 */
gbase::GSet<Signature> nearSignatures(Signature signature, gbase::Integer distance);

/**
 * @brief Used to identify the style of writing note names. NoteNameStyle::flat uses the suffix b to indicate
 * flattened notes. NoteNameStyle::sharp uses the suffix # to indicate sharpened notes.
 */
enum class NoteNameStyle { flat, sharp };
constexpr auto DefaultNoteNameStyle = NoteNameStyle::flat;
constexpr std::ostream &operator<<(std::ostream &s, const NoteNameStyle &nns) {
    if (nns == NoteNameStyle::flat) {
        s << "flat";
    }
    s << "sharp";
    return s;
}

/**
 * @brief Analyzes a range of NoteNames to see if it uses flat 'b' notation or sharp '#' notation for
 * non-diatonic notes.
 *
 * If either 'b' or '#' can be found, this function will return NoteNameStyle::flat.
 *
 * @param noteNames A range of note names.
 * @return NoteNameStyle::flat or NoteNameStyle::sharp.
 */
template <RangeOfNoteNames Range> constexpr NoteNameStyle noteNameStyle(const Range &noteNames) {

    auto findSharp = [](const String &str) { return str.find('#') != String::npos; };
    const bool containsSharp = std::any_of(noteNames.begin(), noteNames.end(), findSharp);

    if (containsSharp) {
        return NoteNameStyle::sharp;
    }

    return NoteNameStyle::flat;
};

/**
 * @brief Analyzes a single NoteName to see if it uses flat 'b' notation or sharp '#' notation.
 *
 * @param noteName The note name to be analyzed.
 * @return NoteNameStyle::flat or NoteNameStyle::sharp.
 */
constexpr NoteNameStyle noteNameStyle(const NoteName &noteName) {
    NoteNames v;
    v.pushBack(noteName);
    return noteNameStyle(v);
}

/**
 * @brief Converts a single note value to a note name.
 *
 * @param noteValue The note value to be converted.
 * @param style NoteNameStyle::flat or NoteNameStyle::sharp.
 * @param showOctave If true, the octave number of the note will be added as postfix.
 * @return NoteName
 */
NoteName noteName(NoteValue noteValue, NoteNameStyle style = DefaultNoteNameStyle, bool showOctave = true);

/**
 * @brief Converts a single note name to its note value.
 *
 * @param noteName The note name to be converted, e.g. "Db" or "C#2".
 * @return NoteValue
 */
NoteValue noteValue(const NoteName &noteName);

/**
 * @brief Converts a note value OR a note name to its note value.
 *
 * @tparam Type
 * @param note Note value OR note name.
 * @return NoteValue
 */
template <typename Type> constexpr NoteValue noteToNoteValue(const Type &note) {
    if constexpr (std::is_same_v<Type, NoteValue>) {
        return note;
    } else if constexpr (std::is_same_v<Type, NoteName>) {
        return noteValue(note);
    }

    GTHROW(GInvalidArgument, "Argument must be NoteValue or NoteName: ", note, ", ", typeName(note));
}

/**
 * @brief Transforms note values to note names.
 *
 * Example usage:
 * @code
 * auto noteNames = noteValues | noteValueToNoteName(NoteNameStyle::sharp);
 * @endcode
 *
 * @param style NoteNameStyle::flat or NoteNameStyle::sharp.
 * @param showOctave If true, the octave number of the note will be added as suffix.
 * @return A function which converts a note value to its note name.
 */
constexpr auto noteValueToNoteName(NoteNameStyle style = DefaultNoteNameStyle, bool showOctave = true) {
    auto fn = std::ranges::views::transform(
        [=](NoteValue noteValue) { return noteName(noteValue, style, showOctave); });
    return fn;
}

/**
 * @brief Transforms note names to note values.
 *
 * Example usage:
 * @code
 * auto noteValues = noteNames | noteNameToNoteValue;
 * @endcode
 */
constexpr auto noteNameToNoteValue =
    std::ranges::views::transform([](NoteName noteName) { return noteValue(noteName); });

/**
 * @brief Returns a view of sequential note values.
 *
 * Example usage:
 * @code
 * for (auto n : noteValues(0, NoteIterval::Octave)) {
 *     cout << n << endl;
 * }
 * @endcode
 * @param firstNoteValue The first note value.
 * @param numberNotes The number of note values to be produced.
 * @return View of sequential NoteValue.
 */
constexpr auto noteValues(NoteValue firstNoteValue, gbase::Size numberNotes) {
    return std::views::iota(firstNoteValue, static_cast<NoteValue>(firstNoteValue + numberNotes));
}

/**
 * @brief Creates a range of note values from an initializer list of note names.
 *
 * Example usage:
 * @code
 * NoteValues noteValues = noteValues{"C0", "E0", "F0"};
 * @endcode
 */
constexpr NoteValues noteValues(std::initializer_list<NoteName> noteNames) {
    NoteValues result;
    for (auto value : noteNames | noteNameToNoteValue) {
        result.pushBack(value);
    }
    return result;
}

/**
 * @brief Returns a view of sequential note names.
 *
 * @param firstNoteValue The note value of the first note.
 * @param numberNotes The number of note values to be produced.
 * @param style NoteNameStyle::flat or NoteNameStyle::sharp.
 * @param showOctave If true, the octave number of the note will be added as suffix.
 * @return View of sequential NoteName.
 */
constexpr auto noteNames(NoteValue firstNoteValue, Size numberNotes,
                         NoteNameStyle style = DefaultNoteNameStyle, bool showOctave = true) {
    return noteValues(firstNoteValue, numberNotes) | noteValueToNoteName(style);
}

/**
 * @brief Sorts the given note names in ascending order.
 *
 * @tparam Range
 * @param noteNames A range of NoteName to be sorted.
 * @return A sorted GVector with note names.
 */
template <RangeOfNoteNames Range> constexpr NoteNames sortNoteNames(const Range &noteNames) {

    NoteNames result{noteNames};
    auto sortFn = [](const NoteName &a, const NoteName &b) { return noteValue(a) < noteValue(b); };
    result.sort(sortFn);
    return result;
}

/**
 * @brief Strips the octave suffix from note names.
 *
 * Example usage:
 * @code
 * auto noteNamesWithoutOctaveSuffix = noteNamesWithOctaveSuffix | stripOctaveFromNoteName;
 * @endcode
 */
constexpr auto stripOctaveFromNoteName = std::ranges::views::transform([](NoteName noteName) {
    NoteName result{noteName};
    result.erase(std::remove_if(result.begin(), result.end(), ::isdigit), result.end());
    return result;
});

/**
 * @brief Tests if a given note name represents a white key on the piano.
 */
constexpr bool isDiatonic(const NoteName &noteName) {
    return !(noteName.contains("b") || noteName.contains("#"));
}

/**
 * @brief Tests if a given note value represents a white key on the piano.
 */
constexpr bool isDiatonic(const NoteValue &noteValue) { return isDiatonic(noteName(noteValue)); }

/**
 * @brief Finds the C-note base value for given note values.
 *
 * The C-note base is the C-note which is closest to to the given note values but
 * also has a lower value than all given note values.
 */
template <RangeOfNoteValues Range> constexpr NoteValue baseNoteValue(const Range &noteValues) {

    const auto minNoteValue = std::ranges::min(noteValues);
    return (minNoteValue / NI::Octave) * NI::Octave;
}

/**
 * @brief Finds the C-note base value for series of harmonies.
 *
 * The C-note base is the C-note which is closest to to the note values of the harmony sequence but
 * also has a lower value than all given note values.
 */
template <RangeOfRangeOfNoteValues Range> constexpr NoteValue baseNoteValue(const Range &harmonySequence) {
    GVector<NoteValue> mins;
    for (const auto &noteValues : harmonySequence) {
        mins.pushBack(NoteValue{std::ranges::min(noteValues)});
    }
    return (std::ranges::min(mins) / NI::Octave) * NI::Octave;
}

/**
 * @brief Transform for rebasing a range of note values.
 *
 * Example usage:
 * @code
 * NoteValues noteValuesToBeRebased;
 * const NoteValue newBase{noteValue{NoteName{"C2"}}};
 * const NoteValue oldBase{baseNoteValue(noteValuesToBeRebased)};
 * const NoteValues rebasedNoteValues = noteValuesToBeRebased | rebaseNoteValue(newBase, oldBase);
 * @endcode
 *
 * @param targetBaseNoteValue The new base note value - must be a C-note!
 * @param currentBaseNoteValue The current base note value - can be found with baseNoteValue();
 */
constexpr auto rebaseNoteValue(NoteValue targetBaseNoteValue, NoteValue currentBaseNoteValue) {

    if ((targetBaseNoteValue % NI::Octave != 0) || (currentBaseNoteValue % NI::Octave != 0)) {
        GTHROW(GInvalidArgument, "Base notes for rebasing must be a C-note: ", noteName(targetBaseNoteValue),
               noteName(currentBaseNoteValue));
    }

    auto fn = std::ranges::views::transform([=](NoteValue noteValue) {
        const auto zeroBasedNoteValue = noteValue - currentBaseNoteValue;
        return zeroBasedNoteValue + targetBaseNoteValue;
    });
    return fn;
}

/**
 * @brief Rebases a range of note values to a new base note.
 *
 * @tparam Range
 * @param noteValues A range of note values.
 * @param newBaseNoteValue The new base note - must be a C-note!
 */
template <RangeOfNoteValues Range>
constexpr auto rebaseNoteValues(const Range &noteValues, NoteValue newBaseNoteValue) {

    const auto oldBaseNoteValue = baseNoteValue(noteValues);
    return noteValues | rebaseNoteValue(newBaseNoteValue, oldBaseNoteValue);
}

/**
 * @brief Rebases a sequence of note value ranges.
 *
 * @tparam RangeOfRange
 * @param sequence A range which contains ranges of note values.
 * @param newBaseNoteValue The new base note - must be a C-note!
 */
template <RangeOfRangeOfNoteValues RangeOfRange>
constexpr auto rebaseNoteValues(const RangeOfRange &sequence, NoteValue newBaseNoteValue) {

    GVector<NoteValues> result;

    const auto oldBaseNoteValue = baseNoteValue(sequence);

    for (const auto &noteValues : sequence) {
        result.pushBack(GVector<NoteValue>{noteValues | rebaseNoteValue(newBaseNoteValue, oldBaseNoteValue)});
    }

    return result;
}
} // namespace gmtheory