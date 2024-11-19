#ifndef MIDI_GENERATOR_H
#define MIDI_GENERATOR_H

#include "taal.h"
#include <string>

class MIDIGenerator {
public:
    void generateMIDI(const Taal &taal, const std::string &filename);

private:
    int getMIDIPitch(const std::string &bol);
};

#endif // MIDI_GENERATOR_H
