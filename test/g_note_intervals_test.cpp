#include <ranges>

#include "g_note_intervals.hpp"
#include "g_test_framework.hpp"

using namespace gbase;
namespace test {

using namespace gmtheory;
using namespace std;

GTEST(NoteIntervalTest) {
    GCHECK("Short names", NoteIntervalShortNames[NI::Octave], String("O"));

    const NoteValues v1 = {0, 5, 13, 98};
    const NoteValues resultNormalized(v1 | normalize);
    const NoteValues expectedNormalized = {0, 5, 1, 2};
    GCHECK("Normalize", resultNormalized, expectedNormalized);

    const NoteValues resultTransposed(v1 | transpose(2));
    NoteValues expectedTransposed = {2, 7, 15, 100};
    GCHECK("Transpose", resultTransposed, expectedTransposed);

    const NoteValues resultNormalizedAndTransposed(v1 | normalize | transpose(2));
    const NoteValues expectedNormalizedAndTransposed = {2, 7, 3, 4};
    GCHECK("Normalize | Transpose", resultNormalizedAndTransposed, expectedNormalizedAndTransposed);

    const NoteValues resultTransposedAndNormalized(v1 | transpose(2) | normalize);
    const NoteValues expectedTransposedAndNormalized = {2, 7, 3, 4};
    GCHECK("Transpose | Normalize", resultTransposedAndNormalized, expectedTransposedAndNormalized);

    const NoteValues resultOctaveTransposition(v1 | octaveTransposition(1));
    NoteValues expectedOctaveTransposition = {12, 17, 25, 110};
    GCHECK("Transpose", resultOctaveTransposition, expectedOctaveTransposition);
}

} // namespace test