#include "taal.h"
#include <iostream>

int main() {
    TaalManager manager;

    // Load Tals from JSON
    try {
        manager.loadTalsFromJson("tals.json");
    } catch (const std::exception& e) {
        std::cerr << "Error loading Tals: " << e.what() << std::endl;
    }

    // Add a new Taal
    manager.addTaal("CustomTaal", {"Dha", "Dha", "Tin", "Na", "Dhin"});

    // Save updated Tals back to JSON
    try {
        manager.saveTalsToJson("tals.json");
    } catch (const std::exception& e) {
        std::cerr << "Error saving Tals: " << e.what() << std::endl;
    }

    // Print all Tals
    for (const auto& [name, taal] : manager.getAllTals()) {
        std::cout << "Taal: " << name << ", Beats: ";
        for (const auto& bol : taal.getBols()) {
            std::cout << bol << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
