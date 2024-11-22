#include "MIDIHandler.h"
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <cstdint>
#include <stdexcept>

namespace {
    void writeVarLen(std::vector<uint8_t>& trackData, uint32_t value) {
        uint8_t buffer[4];
        int i = 0;

        do {
            buffer[i++] = value & 0x7F;
            value >>= 7;
        } while (value > 0);

        for (int j = i - 1; j > 0; --j) {
            trackData.push_back(buffer[j] | 0x80);
        }
        trackData.push_back(buffer[0]);
    }

    void writeHeaderChunk(std::ofstream& out) {
        out.write("MThd", 4); // Header chunk identifier
        uint32_t headerLength = 6;
        out.write(reinterpret_cast<const char*>(&headerLength), sizeof(headerLength)); // Header length
        uint16_t format = 0; // Format type (0 = single track)
        out.write(reinterpret_cast<const char*>(&format), sizeof(format));
        uint16_t nTracks = 1; // Number of tracks
        out.write(reinterpret_cast<const char*>(&nTracks), sizeof(nTracks));
        uint16_t division = 480; // Pulses per quarter note (PPQN)
        out.write(reinterpret_cast<const char*>(&division), sizeof(division));
    }

    void writeTrackChunk(std::ofstream& out, const std::vector<uint8_t>& trackData) {
        out.write("MTrk", 4); // Track chunk identifier
        uint32_t length = trackData.size();
        out.write(reinterpret_cast<const char*>(&length), sizeof(length)); // Track length
        out.write(reinterpret_cast<const char*>(trackData.data()), trackData.size());
    }

    void writeNoteEvent(std::vector<uint8_t>& trackData, uint8_t deltaTime, uint8_t channel, uint8_t note, uint8_t velocity, bool isNoteOn) {
        writeVarLen(trackData, deltaTime);
        trackData.push_back((isNoteOn ? 0x90 : 0x80) | channel); // Note On/Off + Channel
        trackData.push_back(note);
        trackData.push_back(velocity);
    }

    void writeProgramChange(std::vector<uint8_t>& trackData, uint8_t channel, uint8_t program) {
        trackData.push_back(0xC0 | channel); // Program Change + Channel
        trackData.push_back(program); // Program number
    }
}

void MIDIHandler::generateTaalMIDI(const Taal& taal, const Tempo& tempo, const std::string& raag, const std::string& outputPath) const {
    std::ofstream midiFile(outputPath, std::ios::binary);
    if (!midiFile.is_open()) {
        throw std::runtime_error("Failed to open MIDI file for writing: " + outputPath);
    }

    // Write the MIDI header chunk
    writeHeaderChunk(midiFile);

    // Create the track data
    std::vector<uint8_t> trackData;

    // Add a tempo event (Meta Event FF 51 03 tttttt)
    trackData.push_back(0x00); // Delta time
    trackData.push_back(0xFF); // Meta event
    trackData.push_back(0x51); // Tempo
    trackData.push_back(0x03); // Length of tempo data
    uint32_t tempoValue = 60000000 / tempo.bpm; // Microseconds per quarter note
    trackData.push_back((tempoValue >> 16) & 0xFF);
    trackData.push_back((tempoValue >> 8) & 0xFF);
    trackData.push_back(tempoValue & 0xFF);

    // Program change: Assign Standard Drum Kit to channel 10
    writeProgramChange(trackData, 9, 0); // Channel 10 (0-based), Program 0

    // Note mapping for bols (General MIDI percussion keys)
    std::unordered_map<std::string, uint8_t> bolToNote = {
        {"Dha", 36}, {"Dhin", 38}, {"Na", 40}, {"Ti", 42}, {"Ge", 44},
        {"Ka", 46}, {"Ta", 48}, {"Tom", 50}, {"Nam", 52}, {"Jo", 54},
        {"Nu", 56}, {"Di", 58}, {"Mi", 60}
    };

    uint8_t velocity = 80; // Default velocity for notes
    uint8_t duration = 480 / taal.beats; // Note duration in ticks

    // Generate note events for the Taal
    for (int cycle = 0; cycle < 4; ++cycle) { // Play Taal 4 times
        for (const auto& bol : taal.bols) {
            uint8_t note = bolToNote.count(bol) ? bolToNote[bol] : 60; // Default to middle C if bol not found
            writeNoteEvent(trackData, 0, 9, note, velocity, true);  // Note On (channel 10)
            writeNoteEvent(trackData, duration, 9, note, velocity, false); // Note Off (channel 10)
        }
    }

    // End of track event (Meta Event FF 2F 00)
    trackData.push_back(0x00); // Delta time
    trackData.push_back(0xFF); // Meta event
    trackData.push_back(0x2F); // End of track
    trackData.push_back(0x00); // Length

    // Write the track chunk
    writeTrackChunk(midiFile, trackData);

    std::cout << "MIDI file generated: " << outputPath << std::endl;
}
