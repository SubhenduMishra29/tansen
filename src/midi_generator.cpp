#include "midi_generator.h"
#include <RtMidi.h>
#include <fstream>

void MIDIGenerator::generateMIDI(const Taal &taal, const std::string &filename) {
    std::ofstream midiFile(filename, std::ios::binary);

    // MIDI Header
    midiFile << "MThd" << char(0) << char(0) << char(0) << char(6) << char(0)
             << char(1) << char(0) << char(1) << char(0) << char(96);

    // MIDI Track
    midiFile << "MTrk";

    // Generate notes from Taal
    for (const auto &bol : taal.bols) {
        int pitch = getMIDIPitch(bol);
        midiFile.put(pitch);
        midiFile.put(0x90);  // Note On
        midiFile.put(pitch);
        midiFile.put(127);   // Velocity
        midiFile.put(0x80);  // Note Off
        midiFile.put(pitch);
        midiFile.put(0);
    }

    midiFile.close();
}

int MIDIGenerator::getMIDIPitch(const std::string &bol) {
    static std::map<std::string, int> pitchMap = {
        {"Dha", 60}, {"Dhin", 62}, {"Na", 64}, {"Tin", 65}
    };
    return pitchMap[bol];
}
