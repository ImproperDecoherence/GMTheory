
#include <iostream>

#include "gnoteintervals.hpp"

namespace gmtheory {

const gbase::GDictionary<NoteInterval, NoteName> NoteIntervalShortNames = {
    {NI::R, "R"},       {NI::m2, "m2"}, {NI::M2, "M2"}, {NI::m3, "m3"},   {NI::M3, "M3"},  {NI::P4, "P4"},
    {NI::dim5, "dim5"}, {NI::P5, "P5"}, {NI::m6, "m6"}, {NI::M6, "M6"},   {NI::m7, "m7"},  {NI::M7, "M7"},
    {NI::O, "O"},       {NI::m9, "m9"}, {NI::M9, "M9"}, {NI::P11, "P11"}, {NI::M13, "M13"}};

const NoteName &noteIntervalShortName(NoteValue interval) { return NoteIntervalShortNames[interval]; }

} // namespace gmtheory