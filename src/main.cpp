#include "midi_generator.h"
#include "taal.h"
#include "rhythm_pattern.h"

int main() {
    // Initialize Taal system
    TaalManager taalManager;
    taalManager.loadDefaultTals();

    // Create MIDI Generator
    MIDIGenerator midiGenerator;

    // Select a Taal
    auto taal = taalManager.getTaal("Teentaal");
    if (!taal) {
        std::cerr << "Error: Taal not found!" << std::endl;
        return 1;
    }

    // Generate MIDI for the Taal
    midiGenerator.generateMIDI(*taal, "output_teentaal.mid");

    std::cout << "MIDI file generated: output_teentaal.mid" << std::endl;
    return 0;
}
