#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <ctime>

// Define Taal (rhythmic cycle) structure
struct Taal {
    std::string name;
    std::vector<std::string> bols;
    int beats; // Number of beats in the Taal
};

// Tempo structure to store tempo details
struct Tempo {
    std::string name;
    int beatsPerMinute; // BPM
};

// Instrument structure to represent different instruments
struct Instrument {
    std::string name;
    int midiChannel; // MIDI channel for instrument
    int pitch; // Pitch for the instrument (for example, tabla or tanpura)
};

// Rhythmic variations (Dugun, Tigun, Chauguni)
enum RhythmicVariation {
    NORMAL, DUGUN, TIGUN, CHAUGUNI
};

// Define a class to manage the music system
class MusicSystem {
public:
    MusicSystem() {
        // Populate Taal list
        taals = {
            {"Teentaal", {"Dha", "Dhin", "Dha", "Dhin", "Dha", "Dhin", "Dha", "Dhin", "Na", "Dha", "Na", "Dha"}, 16},
            {"Keherwa", {"Dha", "Dhin", "Na", "Dhin", "Dha", "Dhin", "Na", "Dhin"}, 8},
            {"Ektaal", {"Dha", "Dhin", "Dha", "Dhin", "Na", "Dhin", "Na", "Dha", "Dhin", "Na", "Dhin", "Na", "Dha", "Na", "Dha", "Na"}, 12},
            {"Rupak", {"Dha", "Na", "Dhin", "Na", "Dha", "Na"}, 7},
            {"Jhaptaal", {"Dha", "Dhin", "Na", "Na", "Dhin", "Dha", "Dhin", "Na"}, 10},
            {"Adi", {"Dha", "Dhin", "Dha", "Dhin", "Dha", "Dhin", "Dha", "Dhin"}, 8},
            {"Chautal", {"Dha", "Na", "Dhin", "Dha", "Na", "Na", "Dha", "Dhin", "Na", "Na"}, 12}
        };

        // Populate Tempo list
        tempos = {
            {"Slow", 60},
            {"Medium", 90},
            {"Fast", 120},
            {"Very Fast", 150}
        };

        // Default Instrument setup
        instruments = {
            {"Tabla", 1, 60},  // Example pitch for Tabla
            {"Santoor", 2, 64}, // Example pitch for Santoor
            {"Tanpura", 3, 48}, // Example pitch for Tanpura
        };
    }

    // Select Taal
    void selectTaal(const std::string& taalName) {
        if (taals.find(taalName) != taals.end()) {
            currentTaal = taals[taalName];
            std::cout << "Selected Taal: " << taalName << " with " << currentTaal.beats << " beats\n";
        } else {
            std::cout << "Taal not found.\n";
        }
    }

    // Select Tempo
    void selectTempo(const std::string& tempoName) {
        if (tempos.find(tempoName) != tempos.end()) {
            currentTempo = tempos[tempoName];
            std::cout << "Selected Tempo: " << tempoName << " (" << currentTempo.beatsPerMinute << " BPM)\n";
        } else {
            std::cout << "Tempo not found.\n";
        }
    }

    // Select Instrument
    void selectInstrument(const std::string& instrumentName) {
        if (instruments.find(instrumentName) != instruments.end()) {
            currentInstrument = instruments[instrumentName];
            std::cout << "Selected Instrument: " << instrumentName << "\n";
        } else {
            std::cout << "Instrument not found.\n";
        }
    }

    // Apply Rhythmic Variation
    void applyVariation(RhythmicVariation variation) {
        variationType = variation;
        std::string variationName;
        switch (variation) {
            case DUGUN: variationName = "Dugun"; break;
            case TIGUN: variationName = "Tigun"; break;
            case CHAUGUNI: variationName = "Chauguni"; break;
            case NORMAL: variationName = "Normal"; break;
        }
        std::cout << "Applied Rhythmic Variation: " << variationName << "\n";
    }

    // Generate MIDI file for the selected Taal and Tempo
    void generateMIDI(const std::string& filename) {
        std::ofstream midiFile(filename);
        if (midiFile.is_open()) {
            // Write header for MIDI (basic, for example purposes)
            midiFile << "MIDI File: Generated from Indian Classical Music System\n";
            midiFile << "Taal: " << currentTaal.name << "\n";
            midiFile << "Tempo: " << currentTempo.name << " (" << currentTempo.beatsPerMinute << " BPM)\n";
            midiFile << "Instrument: " << currentInstrument.name << "\n";
            midiFile << "Rhythmic Variation: " << (variationType == NORMAL ? "Normal" : (variationType == DUGUN ? "Dugun" : (variationType == TIGUN ? "Tigun" : "Chauguni"))) << "\n\n";
            midiFile << "Pattern:\n";

            // Generate the rhythmic pattern based on selected Taal
            int ticksPerBeat = 480;
            int duration = ticksPerBeat * (60 / currentTempo.beatsPerMinute);

            for (const std::string& bol : currentTaal.bols) {
                midiFile << bol << " (Duration: " << duration << " ticks)\n";
                // Map bol to MIDI note, this is a simplified mapping
                if (bol == "Dha") {
                    midiFile << "Note: " << currentInstrument.pitch << "\n";
                }
                // Handle other bols as needed (Na, Dhin, etc.)
            }

            // Close the file
            midiFile.close();
            std::cout << "MIDI file generated: " << filename << "\n";
        } else {
            std::cout << "Failed to create MIDI file.\n";
        }
    }

private:
    // Store information for Taal, Tempo, Instruments, etc.
    std::unordered_map<std::string, Taal> taals;
    std::unordered_map<std::string, Tempo> tempos;
    std::unordered_map<std::string, Instrument> instruments;

    Taal currentTaal;
    Tempo currentTempo;
    Instrument currentInstrument;
    RhythmicVariation variationType = NORMAL; // Default: Normal

};

// Main program to use the Music System
int main() {
    MusicSystem musicSystem;

    // Example usage
    musicSystem.selectTaal("Teentaal");
    musicSystem.selectTempo("Medium");
    musicSystem.selectInstrument("Tabla");
    musicSystem.applyVariation(DUGUN);

    // Generate a MIDI file
    musicSystem.generateMIDI("output.mid");

    return 0;
}
