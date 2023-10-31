/*
  ==============================================================================

    Chord.cpp
    Created: 3 Jun 2017 4:15:43pm
    Author:  blist

  ==============================================================================
*/

#include "Chord.h"
Bach::Chord::Chord() // C
{
	octave = 5;
	setChord("C");
}
Bach::Chord::Chord(String chordName) // A#m7
{
	octave = 5;
	setChord(chordName);
}

void Bach::Chord::setChord(String chordName) // A#m7/B
{
	root = chordNameToMidiNotes.getChordRoot(chordName);
	type = chordNameToMidiNotes.getChordType(chordName);
	bass = chordNameToMidiNotes.getChordBass(chordName);
}

void Bach::Chord::setRoot(String rootNote) // A#
{
	bass = rootNote;
}

void Bach::Chord::setType(String chordType) // m7
{
	type = chordType;
}

void Bach::Chord::setBass(String bassNote) // B
{
	bass = bassNote;
}

void Bach::Chord::setInversions(int inversions)
{
    this->inversions = inversions;
}

void Bach::Chord::setOctave(int octave)
{
    this->octave = octave;
}

void Bach::Chord::setVoicing(Voicing voicing)
{
    this->voicing = voicing;
}

OwnedArray<Bach::Note> Bach::Chord::getNotes()
{
    OwnedArray<Bach::Note> notes;
    auto midiNotes = Bach::Chord::getMidiNoteNumbers();
    for (auto midiNote : midiNotes) {
        auto note = new Bach::Note(midiNote);
        notes.add(note);
    }
    return notes;
}

Array<int> Bach::Chord::getMidiNoteNumbers() // [48, 52, 55]
{
	Array<int> notes = chordNameToMidiNotes.get(root, type, bass, octave, inversions);
    //apply voicing if needed
    notes = getNotesWithCustomVoicing(notes);
	return notes;
}

Array<String> Bach::Chord::getPitchClassWithOctaveNotes() // [A3, C4, E4]
{
    Array<String> pitchClassWithOctaveNotes;
    auto notes = getNotes();
    for (int i = 0; i < notes.size(); i++) {
        pitchClassWithOctaveNotes.add(notes[i]->getPitchClassWithOctave());
    }
    return pitchClassWithOctaveNotes;
}

Array<String> Bach::Chord::getPitchClassNotes() // [A, C, E]
{
    Array<String> pitchClassNotes;
    auto notes = getNotes();
    for (int i = 0; i < notes.size(); i++) {
        pitchClassNotes.add(notes[i]->getPitchClass());
    }
	return pitchClassNotes;
}

Array<int> Bach::Chord::getIntegerPitchClassNotes() // [0, 3, 5, 7]
{
    Array<int> integerPitchClassNotes;
    auto notes = getNotes();
    for (int i = 0; i < notes.size(); i++) {
        integerPitchClassNotes.add(notes[i]->getIntegerPitchClass());
    }
    return integerPitchClassNotes;
}

Array<String> Bach::Chord::getIntervals() // [P1, m3, P5]
{
    Array<String> intervals;
    auto integerNotes = getIntegerPitchClassNotes();
    if (integerNotes.size() == 0) return {};

    int rootNote = integerNotes[0];
    for (int i = 0; i < integerNotes.size(); i++) {
        int interval = integerNotes[i] - rootNote;
        String intervalName;
        switch (interval) {
            case 0: intervalName = "P1"; break;
            case 1: intervalName = "m2"; break;
            case 2: intervalName = "M2"; break;
            case 3: intervalName = "m3"; break;
            case 4: intervalName = "M3"; break;
            case 5: intervalName = "P4"; break;
            case 6: intervalName = "d5"; break;
            case 7: intervalName = "P5"; break;
            case 8: intervalName = "m6"; break;
            case 9: intervalName = "M6"; break;
            case 10: intervalName = "m7"; break;
            case 11: intervalName = "M7"; break;
            // Add more intervals as needed
            default: intervalName = "Unknown"; break;
        }
        intervals.add(intervalName);
    }
    return intervals;
}

Array<int> Bach::Chord::getNotesWithCustomVoicing(Array<int> notes)
{
    Array<int> notesToAdd;

    for (const auto& op : voicing.operations)
    {
       if (op.type == "duplicate")
        {
            if (op.index < notes.size())
            {
                int note = notes[op.index];
                for (int octave : op.octaves)
                {
                    notesToAdd.add(note + (octave * 12));
                }
            }
        }
    }

    // Handle 'drop' operations
    for (const auto& op : voicing.operations)
    {
        if (op.type == "drop")
        {
            // Sort and iterate in reverse to avoid index issues
            Array<int> sortedDrop = op.drop;
            sortedDrop.sort();
            for (int i = sortedDrop.size() - 1; i >= 0; --i)
            {
                int index = sortedDrop[i];
                if (index < notes.size())
                    notes.remove(index);
            }
        }
    }

    // Now add any notes that need to be added
    notes.addArray(notesToAdd);

    return notes;
}
