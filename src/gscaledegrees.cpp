
#include "gscaledegrees.hpp"

namespace gmtheory {

const GDictionary<GScaleDegree, String> scaleDegreeNames = {
    {GScaleDegree::Tonic, "Tonic"},       {GScaleDegree::Supertonic, "Supertonic"},
    {GScaleDegree::Mediant, "Mediant"},   {GScaleDegree::Subdominant, "Subdominant"},
    {GScaleDegree::Dominant, "Dominant"}, {GScaleDegree::Submediant, "Submediant"},
    {GScaleDegree::Subtonic, "Subtonic"}};

}