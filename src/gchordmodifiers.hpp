/**
 * @file gchordmodifiers.hpp
 * @author ImproperDecoherence (gustowny@gmail.com)
 * @brief Defines the different modifiers which can be applied to a chord.
 * @version 0.1
 * @date 2024-10-28
 *
 * Copyright (c) 2024-2024 Jonas Gustavsson
 *
 */

#include <ranges>

#include "gdictionary.hpp"
#include "gharmony.hpp"
#include "gnoteintervals.hpp"
#include "gnotes.hpp"
#include "gstringtools.hpp"

#pragma once

using namespace gbase;

namespace gmtheory {

/**
 * @brief Code that represents zero, one or several chord modifications. Each binary 1 represents a specific
 * chord modification.
 */
using GModFlag = Unsigned;

/**
 * @brief Binary unique identifiers for available chord modifiers. Chord modifiers can be combinated using the
 * binary | operator.
 *
 * Example usage:
 * @code {.language-id}
 * const GModFlag mod7add2 = CMF::Dominant7 | CMF::Add2;
 * @endcode
 */
namespace CMF {
// clang-format off
  constexpr GModFlag NoFlag       = 0b0000000000000000;
  constexpr GModFlag Dominant7    = 0b0000000000000001;
  constexpr GModFlag Major7       = 0b0000000000000010;
  constexpr GModFlag Dominant9    = 0b0000000000000100;
  constexpr GModFlag Dominant11   = 0b0000000000001000;
  constexpr GModFlag Dominant13   = 0b0000000000010000;
  constexpr GModFlag Add2         = 0b0000000000100000;
  constexpr GModFlag Add6         = 0b0000000001000000;
  constexpr GModFlag Add9         = 0b0000000010000000;
  constexpr GModFlag Flat5th      = 0b0000000100000000;
  constexpr GModFlag Suspended2nd = 0b0000001000000000;
  constexpr GModFlag Suspended4th = 0b0000010000000000;
// clang-format on
}; // namespace CMF

/**
 * @brief A set of chord modifier flags.
 */
using GChordModFlags = GSet<GModFlag>;

/**
 * @brief Implements a modifier which can be applied to a chord.
 */
class GChordModifier {

  public:
    /**
     * @brief Constructs a new GChordModifier object.
     *
     * @param longName The long name of the modification, e.g. 'Dominant 7'.
     * @param shortName The short name of the modification, e.g. '7'.
     * @param noteIntervalsToAdd Note intervals to be added by the modifier.
     * @param noteIntervalsToRemove Note intervals to be removed by the modifier.
     * @param modFlagsToBeCancelled Ohter modifications which shall be cancelled by this modification.
     */
    GChordModifier(const String &longName, const String &shortName, const GHarmony &noteIntervalsToAdd,
                   const GHarmony &noteIntervalsToRemove, const GChordModFlags &modFlagsToBeCancelled)
        : longName_{longName}, shortName_{shortName}, noteIntervalsToAdd_{noteIntervalsToAdd},
          noteIntervalsToRemove_{noteIntervalsToRemove}, modFlagsToBeCancelled_{modFlagsToBeCancelled} {}

    GChordModifier() = delete;
    GChordModifier(const GChordModifier &) = default;
    GChordModifier(GChordModifier &&) = default;

    ~GChordModifier() = default;

    /**
     * @brief Applies the modifier to the given harmony of note values.
     *
     * @param harmony The harmony to be altered. It's assumed that the lowest note value is 0, i.e. the tonic
     * is C0.
     * @param tonic The tonic note value to be applied.
     */
    void apply(GHarmony &harmony, NoteValue tonic) const;

    /**
     * @brief Returns the short name of the modifier.
     */
    constexpr const String &shortName() const { return shortName_; }

    /**
     * @brief Returns the long name of the modifier.
     */
    constexpr const String &longName() const { return longName_; }

    /**
     * @brief Appends the short name to the provided string.
     */
    constexpr void appendShortName(String &input) const { input.append(shortName_); }

    /**
     * @brief Appends the long name to the provided string.
     */
    void appendLongName(String &input) const;

    /**
     * @brief Returns identifiers for modifiers which will be cancelled by this modifier.
     */
    constexpr const GChordModFlags &cancelsModifiers() const { return modFlagsToBeCancelled_; }

  private:
    String longName_;
    String shortName_;
    GHarmony noteIntervalsToAdd_;
    GHarmony noteIntervalsToRemove_;
    GChordModFlags modFlagsToBeCancelled_;
};

/**
 * @brief Implementation of avalable chord modifiers.
 */
extern const GDictionary<GModFlag, GChordModifier> ChordModifiers;

/**
 * @brief Converts the binary representaion of chord modifier identifiers to a vector of chord modifier
 * identifiers.
 */
GChordModFlags modFlagToChordModFlags(GModFlag flags);

/**
 * @brief Converts the vector representaion of chord modifier identifiers to the binary representaion of chord
 * modifier identifiers.
 */
GModFlag chordModFlagsToModFlag(const GChordModFlags flags);

} // namespace gmtheory