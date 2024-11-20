#include "MIDIHandler.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdint>

namespace {
    // MIDI Utility Functions
    void writeVarLen(std::ofstream& out, uint32_t value) {
        uint8_t buffer[4];
        int i = 0;

        do {
            buffer[i++] = value & 0x7F;
            value >>= 7;
        } while (value > 0);

        for (int j = i - 1; j > 0; --j) {
            out.put(buffer[j] | 0x80);
        }
        out.put(buffer[0]);
    }

    void writeHeaderChunk(std::ofstream& out) {
        out.write("MThd", 4); // Header chunk identifier
        uint32_t headerLength = 6;
        out.write(reinterpret_cast<const char*>(&headerLength), 4); // Header length
        uint16_t format = 0; // Format type (0 = single track)
        out.write(reinterpret_cast<const char*>(&format), 2);
        uint16_t nTracks = 1; // Number of tracks
        out.write(reinterpret_cast<const char*>(&nTracks), 2);
        uint16_t division = 480; // Pulses per quarter note (PPQN)
        out.write(reinterpret_cast<const char*>(&division), 2);
    }

    void writeTrackChunk(std::ofstream& out, const std::vector<uint8_t>& trackData) {
        out.write("MTrk", 4); // Track chunk identifier
        uint32_t length = trackData.size();
        out.write(reinterpret_cast<const char*>(&length), 4); // Track length
        out.write(reinterpret_cast<const char*>(trackData.data()), trackData.size());
    }

    void writeNoteEvent(std::vector<uint8_t>& trackData, uint8_t deltaTime, uint8_t note, uint8_t velocity, bool isNoteOn) {
        writeVarLen(trackData, deltaTime);
        trackData.push_back(isNoteOn ? 0x90 : 0x80); // Note On/Off
        trackData.push_back(note);
        trackData.push_back(velocity);
    }
}

void MIDIHandler::generateTaalMIDI(const Taal& taal, const Tempo& tempo, const std::string& raag) const {
    std::ofstream midiFile("output/taal_track.mid", std::ios::binary);
    if (!midiFile.is_open()) {
        std::cerr << "Failed to open MIDI file for writing." << std::endl;
        return;
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

    // Note mapping for bols (this can be adjusted based on DAW compatibility)
    std::unordered_map<std::string, uint8_t> bolToNote = {
        {"Dha", 60}, {"Dhin", 62}, {"Na", 64}, {"Ti", 65}, {"Ge", 67},
        {"Ka", 69}, {"Ta", 71}, {"Tom", 72}, {"Nam", 74}, {"Jo", 76},
        {"Nu", 77}, {"Di", 79}, {"Mi", 81}
    };

    uint8_t velocity = 80; // Default velocity for notes
    uint8_t duration = 480 / taal.beats; // Note duration in ticks

    // Generate note events for the Taal
    for (int cycle = 0; cycle < 4; ++cycle) { // Play Taal 4 times
        for (const auto& bol : taal.bols) {
            uint8_t note = bolToNote.count(bol) ? bolToNote[bol] : 60; // Default to middle C if bol not found
            writeNoteEvent(trackData, 0, note, velocity, true);  // Note On
            writeNoteEvent(trackData, duration, note, velocity, false); // Note Off
        }
    }

    // End of track event (Meta Event FF 2F 00)
    trackData.push_back(0x00); // Delta time
    trackData.push_back(0xFF); // Meta event
    trackData.push_back(0x2F); // End of track
    trackData.push_back(0x00); // Length

    // Write the track chunk
    writeTrackChunk(midiFile, trackData);

    std::cout << "MIDI file generated: output/taal_track.mid" << std::endl;
}
