
# GMTheory

GMTheory is a C++ library with functions and data to support building of music theory applications.

## The Current State of GMTheory

GMTheory is currently in alpha version.

## What's GMTheory?

### Notes and Intervals

In GMTheory, notes are represented by the integer type gmtheory::NoteValue. Note value 0 corresponds to the C note in the first octave. Each integer step represents a semitone step in the chromatic scale.

The type gmtheory::NoteName is used to represent the name of a note. Note names can use the gmtheory::NoteNameStyle flat or sharp. Flat notation uses the 'b' character to indicate a flattened note (e.g. 'Gb') while sharp notation uses the '#' character to indicate a shapened note (e.g. 'F#').

![GMTheory note values](https://i.imgur.com/XYJuJgU.png)

Different note intervals are defined in gmtheory::NI, for example gmtheory::NI::minor3rd and gmtheory::NI::Major3rd.

A set of functions and range transforms can be used to manipulate notes, for example:

- gmtheory::transpose
- gmtheory::rebaseNoteValues
- gmtheory::noteValueToNoteName

### Harmonies and Chords

The class gmtheory::GHarmony represents a set of unique note values.

The class gmtheory::GChord represents a chord. A chord is defined by:

- The tonic note.
- The gmtheory::GChordType (major, minor, diminished or augmented).
- A set of chord modifiers, for example 'Dominant 7' or 'Major 7'. Avialable chord modifiers are defined in gmtheory::CMF.

The gmtheory::GChordDatabase can be used to match a set of notes to chords.

### Scales

The class gmtheory::GScale can be used to define a specific note scale. Available scales are defined by gmtheory::GScaleType.
