#include "TaalManager.h"
#include "MIDIHandler.h"
#include "Tempo.h"
#include <iostream>
#include <stdexcept>

// External declarations from parser
extern std::string parsedRaag;
extern std::string parsedTaal;
extern std::string parsedTempo;
extern int yyparse();

int main() {
    TaalManager taalManager;
    MIDIHandler midiHandler;

    // Load Taals
    try {
        taalManager.loadTaals("data/taals.json");
    } catch (const std::exception& e) {
        std::cerr << "Error loading taals: " << e.what() << std::endl;
        return 1;
    }

    // Display available Taals
    std::cout << "Available Taals:" << std::endl;
    taalManager.listAllTaals();

    // Prompt for user input
    std::cout << "Enter your command (e.g., 'Raag Bhairavi Taal Keherwa Bilambit'): ";

    // Parse the user input
    if (yyparse() != 0) {
        std::cerr << "Error: Failed to parse the command. Please check the syntax." << std::endl;
        return 1;
    }

    // Validate parsed values
    if (!taalManager.isTaalAvailable(parsedTaal)) {
        std::cerr << "Error: Taal '" << parsedTaal << "' is not available. Please choose from the listed Taals." << std::endl;
        return 1;
    }

    if (parsedTempo != "Bilambit" && parsedTempo != "Madhya" && parsedTempo != "Drut") {
        std::cerr << "Error: Tempo '" << parsedTempo << "' is invalid. Valid options are 'Bilambit', 'Madhya', or 'Drut'." << std::endl;
        return 1;
    }

    // Generate MIDI
    int tempoValue = (parsedTempo == "Bilambit" ? 60 : parsedTempo == "Madhya" ? 90 : 120);
    Tempo tempo(parsedTempo, tempoValue);

    try {
        Taal taal = taalManager.getTaal(parsedTaal);
        midiHandler.generateTaalMIDI(taal, tempo, parsedRaag);
    } catch (const std::exception& e) {
        std::cerr << "Error generating MIDI: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "MIDI file generated successfully!" << std::endl;
    return 0;
}
