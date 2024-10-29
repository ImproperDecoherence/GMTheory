/**
 * @file gchords.hpp
 * @author ImproperDecoherence (gustowny@gmail.com)
 * @brief Defines the GChord class and available chord types.
 * @version 0.1
 * @date 2024-10-28
 *
 * Copyright (c) 2024-2024 Jonas Gustavsson
 *
 */

#include <optional>
#include <ranges>

#include "gchordmodifiers.hpp"
#include "gchordtemplate.hpp"
#include "gdictionary.hpp"
#include "gharmony.hpp"
#include "gvector.hpp"

#pragma once

using namespace gbase;

namespace gmtheory {

/**
 * @brief Available types of triad chords.
 */
enum class GChordType { Major, Minor, Diminished, Augmented };

/**
 * @brief Chord templates for all available triad chord types.
 */
extern const GDictionary<GChordType, GChordTemplate> ChordTemplates;

/**
 * @brief Type for the number of inversions which shall be applied to a chord.
 */
using Inversion = Unsigned;

class GChord;

/**
 * @brief Concept for a range of GChords.
 */
template <typename Range>
concept RangeOfChords = RangeOf<Range, GChord>;

/**
 * @brief Represents a chord which can be modified by applying modifiers and inversion.
 */
class GChord {
  public:
    /**
     * @brief A class which observes the state of a GChord object needs to implement selected methods of this
     * interface to get notifications when the state of the GChord state is updated.
     */
    class INotification {
      public:
        /**
         * @brief Called when the state of the connected GChord object has changed.
         */
        virtual constexpr void chordChanged(const GChord &chord) {}
    };

    class Notification {
        Notification() = default;

        /**
         * @brief Connects an observer to the GChord object. The observer needs to implement the INotification
         * interface.
         */
        constexpr void connect(INotification &observer) { observers_.pushBack(&observer); }

      private:
        void constexpr chordChanged(const GChord &chord) {
            for (auto observer : observers_) {
                observer->chordChanged(chord);
            }
        }

        GVector<INotification *> observers_;
        friend GChord;
    };

    /**
     * @brief Any observer needs to connect to this object to receive notifications.
     */
    Notification notifications;

    GChord() = delete;
    ~GChord() = default;

    /**
     * @brief Constructs a new GChord object.
     *
     * @tparam Note A gmtheory::NoteValue or a gmtheory::NoteName.
     * @tparam Mods A gmtheory::GModFlag or gmtheory::GChordModFlags.
     * @param tonic The note name OR note value of the chords tonic note.
     * @param chordType The chord type, e.g. GChordType::Major.
     * @param modFlags Modifiers for the chord as a GModFlag OR a GChordModFlags vector.
     *
     * Example usage:
     * @code
     * const GChord chordBdim{NoteName("B"), GChordType::Diminished, CMF::NoFlag};
     * const GChord chordC7Add2{NoteName("C"), GChordType::Major, CMF::Dominant7 | CMF::Add2};
     * @endcode
     *
     */
    template <typename Note, typename Mods>
    GChord(const Note &tonic, const GChordType &chordType, const Mods &modFlags)
        : tonic_{noteToNoteValue(tonic)}, template_{ChordTemplates[chordType]} {
        setFlags(modFlags);
    }

    GChord(const GChord &) = default;
    GChord(GChord &&) = default;
    GChord &operator=(const GChord &) = default;
    GChord &operator=(GChord &&) = default;

    /**
     * Inversion is ignored when comparing.
     */
    constexpr bool operator==(const GChord &rhs) const { return (noteValues() == rhs.noteValues()); }
    constexpr bool operator!=(const GChord &rhs) const { return !(*this == rhs); }
    constexpr auto operator<=>(const GChord &other) const { return noteValues() <=> other.noteValues(); }

    /**
     * @brief Sets the modifictions of the chord.
     */
    void setFlags(const GChordModFlags &flags);

    /**
     * @brief Sets the modifictions of the chord.
     */
    constexpr void setFlags(const GModFlag flags) { setFlags(modFlagToChordModFlags(flags)); }

    /**
     * @brief Returns the note value of the tonic.
     */
    constexpr NoteValue tonicNoteValue() const { return tonic_; }

    /**
     * @brief Returns the note values of the chord.
     */
    NoteValues noteValues() const;

    /**
     * @brief Returns the number of notes the chord.
     */
    constexpr Size numberOfNotes() const { return noteValues().size(); }

    /**
     * @brief Sets the tonic note value.
     */
    void setTonic(NoteValue noteValue);

    /**
     * @brief Sets the inversions of the chord.
     *
     * @param steps Inversion 0 gives no inversion, inversion 1 means that the highest note of the chord is
     * rotated down to be the base note, etc.
     */
    void setInversion(Inversion steps);

    /**
     * @brief Adds +1 to the current inversion. Will cycle back to 0 inversion when inversion =
     * numberOfNotes().
     */
    constexpr void cycleInversion() { setInversion(inversion_ + 1); }

    /**
     * @brief Returns the short name of the chord without modifiers, i.e. 'C#m' for 'C# minor'
     *
     * @param style NoteNameStyle::flat or NoteNameStyle::sharp.
     */
    constexpr String shortTypeName(NoteNameStyle style = DefaultNoteNameStyle) const {
        return template_.shortName(tonic_, style);
    }

    /**
     * @brief Returns the combined short name of the modifiers and the inversion, without the tonic note name
     * and the chord type, e.g. '7add9/G'.
     *
     * @param style NoteNameStyle::flat or NoteNameStyle::sharp.
     */
    String shortModName(NoteNameStyle style = DefaultNoteNameStyle) const;

    /**
     * @brief Returns the full long name of the chord, including modifiers.
     *
     * @param style NoteNameStyle::flat or NoteNameStyle::sharp.
     */
    String longName(NoteNameStyle style = DefaultNoteNameStyle) const;

    /**
     * @brief Returns the full short name of the chord including modifiers and inversion.
     *
     * @param style NoteNameStyle::flat or NoteNameStyle::sharp.
     */
    constexpr String shortName(NoteNameStyle style = DefaultNoteNameStyle) const {
        return shortTypeName(style) + shortModName();
    }

    /**
     * @brief Returns the number of modifications which are applied to the chord.
     */
    constexpr Size numberOfFlags() const { return modFlags_.size(); }

    /**
     * @brief Resurns the GModFlag which represents all mofifications applied to the chord.
     */
    constexpr GModFlag flags() const { return chordModFlagsToModFlag(modFlags_); }

    /**
     * @brief Prints a textual representsion of the chord state.
     */
    void print(std::ostream &target) const;

  private:
    NoteValue tonic_ = 0;
    const GChordTemplate &template_;
    GChordModFlags modFlags_;
    Inversion inversion_ = 0;
};

constexpr std::ostream &operator<<(std::ostream &s, const GChord &chord) {
    chord.print(s);
    return s;
}

} // namespace gmtheory