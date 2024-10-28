
#include "gchorddatabase.hpp"
#include "galgorithms.hpp"
#include "glogger.hpp"

namespace gmtheory {

const GChordDatabase &GChordDatabase::getInstance() {
    static GChordDatabase instance;
    return instance;
}

Size GChordDatabase::size() const {
    Size count{0};

    for (const auto &entry : data_) {
        count += entry.second.size();
    }

    return count;
}

GChordDatabase::GChordDatabase() {
    GLOG_INFO("Creating chord database ...");

    const GVector<GModFlag> noModFlag{CMF::NoFlag};
    const GVector<GModFlag> availableModFlags = noModFlag + ChordModifiers.keys();
    const auto modFlagCombinations{combinations(availableModFlags, 3)};

    for (const NoteValue tonic : noteValues(0, NI::Octave)) {
        for (const auto &chordType : ChordTemplates) {
            addChord(GChord(tonic, chordType.first, CMF::NoFlag));

            for (auto const &flags : modFlagCombinations) {
                addChord(GChord(tonic, chordType.first, flags));
            }
        }
    }

    GLOG_INFO("  Chords in database           : ", size());
    GLOG_INFO("  Unique signatures in database: ", data_.size());

    GLOG_DETAILS(*this);
}

void GChordDatabase::addChord(const GChord &chord) {
    const Signature sig{signature(chord.noteValues())};

    if (!data_.contains(sig)) {
        data_.emplace(sig, GSet<GChord>());
    }

    GSet<GChord> &entry = data_[sig];

    if (const auto &search = entry.find(chord); search != entry.end()) {
        if (chord.numberOfFlags() < search->numberOfFlags()) {
            // Keep the chord with fewest flags.
            GLOG_DETAILS("  Removing chord: ", *search, " (more flags than new chord)");
            entry.erase(search);

        } else if (chord.numberOfFlags() == search->numberOfFlags()) {
            if (chord.flags() < search->flags()) {
                // Keep chord with prioratized flags
                GLOG_DETAILS("  Removing chord: ", *search, " (better flags than new chord)");
                entry.erase(search);
            }
        }
    }

    GLOG_DETAILS("  Adding chord: ", chord);
    entry.extend(chord);
}

void GChordDatabase::print(ostream &os) const {
    os << "GChordDatabase{" << endl;
    for (const auto &entry : data_) {
        for (const auto &chord : entry.second) {
            os << "  signature: " << entry.first << " | " << chord << endl;
        }
    }
    os << "}GChordDatabase" << endl;
}

} // namespace gmtheory