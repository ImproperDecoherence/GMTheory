/**
 * @file gscaledegrees.hpp
 * @author ImproperDecoherence (gustowny@gmail.com)
 * @brief Defines scale degrees.
 * @version 0.1
 * @date 2024-10-28
 *
 * Copyright (c) 2024-2024 Jonas Gustavsson
 *
 */

#include "gbasictypes.hpp"
#include "gdictionary.hpp"

#pragma once

using namespace gbase;

namespace gmtheory {

/**
 * @brief The note positions in a scale for the different scale degrees.
 */
enum GScaleDegree {
    Tonic = 0,
    Supertonic = 1,
    Mediant = 2,
    Subdominant = 3,
    Dominant = 4,
    Submediant = 5,
    Subtonic = 6
};

/**
 * @brief Translation between GScaleDegree and the corresponding name as a String.
 */
extern const GDictionary<GScaleDegree, String> scaleDegreeNames;

} // namespace gmtheory
