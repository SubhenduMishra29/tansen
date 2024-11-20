#include "TaalManager.h"
#include "MIDIHandler.h"
#include "Tempo.h"
#include <iostream>
#include <stdexcept>

int main() {
    TaalManager taalManager;
    MIDIHandler midiHandler;

    try {
        taalManager.loadTaals("data/taals.json");
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    std::cout << "Available Taals:" << std::endl;
    taalManager.listAllTaals();

    std::cout << "Enter your command (e.g., 'Raag Bhairavi Taal Keherwa Bilambit'): ";
    std::string command;
    std::getline(std::cin, command);

    // Parse command (not shown for brevity) and generate MIDI
    std::string raag = "Bhairavi";  // Extracted from command
    std::string taalName = "Keherwa";  // Extracted from command
    std::string tempoName = "Bilambit";  // Extracted from command

    Tempo tempo(tempoName, (tempoName == "Bilambit" ? 60 : tempoName == "Madhya" ? 90 : 120));
    try {
        Taal taal = taalManager.getTaal(taalName);
        midiHandler.generateTaalMIDI(taal, tempo, raag);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    std::cout << "MIDI file generated successfully!" << std::endl;
    return 0;
}
