/**
 * @file gchorddatabase.hpp
 * @author ImproperDecoherence (gustowny@gmail.com)
 * @brief Defines the GChordDatabase class.
 * @version 0.1
 * @date 2024-10-28
 *
 * Copyright (c) 2024-2024 Jonas Gustavsson
 *
 */

#pragma once

#include <iostream>

#include "gchords.hpp"
#include "gdictionary.hpp"

namespace gmtheory {

using namespace gbase;

/**
 * @brief Implements a chord database.
 *
 * The chord database holds:
 *  - all available GChordType:s (see gchords.hpp)
 *  - all available tonic notes
 *  - all available GModFlag:s (see gchordmodifiers.hpp) with up to 3 different in combination
 */
class GChordDatabase {
  public:
    /**
     * @brief Access handle to the singleton instance of the chord database.
     *
     * The chord database will be initialized at the first call to getInstance().
     */
    static const GChordDatabase &getInstance();

    /**
     * @brief Returns the number of chords in the database.
     */
    Size size() const;

    /**
     * @brief Searches the database for chords that matches the provided range of notes.
     *
     * @param noteValues Provided notes.
     * @param distance When distance = 0, an exact match is required. When the distance = 1, one note may
     * differ from the provided notes and the returned chord, etc.
     */
    template <RangeOfNoteValues Range>
    constexpr GSet<GChord> match(const Range &noteValues, Size distance) const {
        const Signature inputSignature{signature(noteValues)};
        const auto signaturesToMatch{nearSignatures(inputSignature, distance)};
        GSet<GChord> result;

        for (const Signature sig : signaturesToMatch) {
            if (data_.contains(sig)) {
                result.extend(data_[sig]);
            }
        }

        return result;
    }

    void print(ostream &os) const;

  private:
    GChordDatabase();

    void addChord(const GChord &chord);

    GDictionary<Signature, GSet<GChord>> data_;
};

constexpr std::ostream &operator<<(std::ostream &os, const GChordDatabase &db) {
    db.print(os);
    return os;
}

} // namespace gmtheory