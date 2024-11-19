#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <stdexcept>

enum RhythmicVariation {
    DUGUN, TIGUN, CHAUGUNI, NORMAL
};

struct Taal {
    std::string name;
    int beats; // Number of beats in the Taal
    std::vector<std::string> bols; // Boles or syllables of the Taal

    Taal(const std::string& n, int b, const std::vector<std::string>& bl)
        : name(n), beats(b), bols(bl) {}

    void display() const {
        std::cout << "Taal: " << name << " (" << beats << " beats)\n";
        std::cout << "Bols: ";
        for (const auto& bol : bols) {
            std::cout << bol << " ";
        }
        std::cout << "\n";
    }
};

struct Tempo {
    std::string name;
    int beatsPerMinute;

    Tempo(const std::string& n, int bpm) : name(n), beatsPerMinute(bpm) {}
};

struct Instrument {
    std::string name;
    int pitch; // A simple representation of pitch

    Instrument(const std::string& n, int p) : name(n), pitch(p) {}
};

class TaalManager {
private:
    std::unordered_map<std::string, Taal> tals;
    std::unordered_map<std::string, Tempo> tempos;
    std::unordered_map<std::string, Instrument> instruments;

    Taal currentTaal;
    Tempo currentTempo;
    Instrument currentInstrument;
    RhythmicVariation variationType = NORMAL; // Default: Normal

public:
    // Function to add a new Taal
    void addTaal(const Taal& newTaal) {
        if (tals.find(newTaal.name) != tals.end()) {
            throw std::invalid_argument("Taal already exists.");
        }
        tals[newTaal.name] = newTaal;
    }

    // Function to set the current Taal
    void setCurrentTaal(const std::string& taalName) {
        auto it = tals.find(taalName);
        if (it == tals.end()) {
            throw std::invalid_argument("Taal not found.");
        }
        currentTaal = it->second;
    }

    // Function to set the current Tempo
    void setCurrentTempo(const Tempo& tempo) {
        currentTempo = tempo;
    }

    // Function to set the current Instrument
    void setCurrentInstrument(const Instrument& instrument) {
        currentInstrument = instrument;
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
            midiFile << "Rhythmic Variation: " << (variationType == NORMAL ? "Normal" : 
                    (variationType == DUGUN ? "Dugun" : 
                    (variationType == TIGUN ? "Tigun" : "Chauguni"))) << "\n\n";
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

    // Function to list all Tals
    void listAllTals() const {
        for (const auto& entry : tals) {
            entry.second.display();
        }
    }
};

int main() {
    // Sample setup
    TaalManager manager;

    // Adding some tals
    manager.addTaal(Taal("Teentaal", 16, {"Dha", "Dhin", "Dha", "Tin", "Na", "Dhin", "Na", "Dha", "Tin", "Na", "Dha", "Tin", "Na", "Dha", "Dhin", "Dha"}));
    manager.addTaal(Taal("Jhaptaal", 10, {"Dha", "Tin", "Na", "Dhin", "Na", "Dha", "Tin", "Na", "Dhin", "Na"}));
    
    // Setting a Taal, Tempo, and Instrument
    manager.setCurrentTaal("Teentaal");
    manager.setCurrentTempo(Tempo("Fast", 120));
    manager.setCurrentInstrument(Instrument("Tabla", 60));

    // Applying a rhythmic variation
    manager.applyVariation(DUGUN);

    // Generate the MIDI file
    manager.generateMIDI("output.mid");

    return 0;
}
