/**
 * @file g_harmony.hpp
 * @author ImproperDecoherence (gustowny@gmail.com)
 * @brief Defines the GHarmony class.
 * @version 0.1
 * @date 2024-10-28
 *
 * Copyright (c) 2024-2024 Jonas Gustavsson
 *
 */

#include "g_notes.hpp"
#include "g_set.hpp"

#pragma once

using namespace gbase;

namespace gmtheory {

/**
 * @brief GHarmony represents a set of sorted unique note values.
 */
class GHarmony : public GSet<NoteValue> {

    using base = GSet<NoteValue>;

  public:
    using base::GSet;
    ~GHarmony() = default;

    /**
     * @brief Construct a new GHarmony object from a vector of note names.
     */
    GHarmony(const NoteNames &noteNames) : base(noteNames | noteNameToNoteValue) {}

    /**
     * @brief Construct a new GHarmony object from an initialzer list of note names.
     */
    GHarmony(std::initializer_list<NoteName> noteNames) : base(noteNames | noteNameToNoteValue) {}
};

constexpr GHarmony operator+(const GHarmony &harmony, NoteValue value) {
    GHarmony copy{harmony};
    copy += value;
    return copy;
}

constexpr GHarmony operator-(const GHarmony &harmony, NoteValue value) {
    GHarmony copy{harmony};
    copy -= value;
    return copy;
}

constexpr GHarmony operator+(const GHarmony &lhs, const GHarmony &rhs) {
    GHarmony copy{lhs};
    copy += rhs;
    return copy;
}

constexpr GHarmony operator-(const GHarmony &lhs, const GHarmony &rhs) {
    GHarmony copy{lhs};
    copy -= rhs;
    return copy;
}

template <RangeOf<NoteName> Range> constexpr GHarmony operator+(const GHarmony &set, const Range &range) {
    GHarmony copy{set};
    copy.extend(range);
    return copy;
}

template <RangeOf<NoteName> Range> constexpr GHarmony operator-(const GHarmony &set, const Range &range) {
    GHarmony copy{set};
    copy.erase(range);
    return copy;
}

} // namespace gmtheory