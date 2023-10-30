/*
  ==============================================================================

    Chord.cpp
    Created: 3 Jun 2017 4:15:43pm
    Author:  blist

  ==============================================================================
*/

#include "Note.h"
Bach::Note::Note(int midi)
{
	setMidiNoteNumber(midi);
	setVelocity(100);
}

Bach::Note::Note(int midi, int vel)
{
	setMidiNoteNumber(midi);
	setVelocity(vel);
}

int Bach::Note::getMidiNoteNumber() // 48
{
	return midiNoteNumber;
}

void Bach::Note::setMidiNoteNumber(int midi)
{
	midiNoteNumber = midi;
}

int Bach::Note::getVelocity() // 48
{
	return velocity;
}

void Bach::Note::setVelocity(int vel)
{
	velocity = vel;
}

String Bach::Note::getPitchClassWithOctave() // A3
{
    int octave = midiNoteNumber / 12 - 1;
    int pitchClass = midiNoteNumber % 12;
    return noteNames[pitchClass] + String(octave);
}

String Bach::Note::getPitchClass() // A
{
    int pitchClass = midiNoteNumber % 12;
    return noteNames[pitchClass];
}

int Bach::Note::getIntegerPitchClass() // 0
{
    return midiNoteNumber % 12;
}

int Bach::Note::getOctave() // 4
{
    return midiNoteNumber / 12 - 1;
}
