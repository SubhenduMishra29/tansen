#include "taal.h"
#include <iostream>

void TaalManager::loadDefaultTals() {
    // Hindustani Tals
    addTaal("Teentaal", {"Dha", "Dhin", "Dhin", "Dha", "Na", "Tin", "Na", "Dhin"});
    addTaal("Jhaptaal", {"Dha", "Dhin", "Dha", "Tin", "Na", "Dhin", "Na", "Dha"});

    // Carnatic Tals
    addTaal("Adi Taal", {"Dha", "Dhin", "Na", "Dhin", "Dha", "Tin", "Na", "Dhin"});
    addTaal("Rupakam", {"Dha", "Na", "Tin", "Dha", "Dhin", "Na"});

    std::cout << "Default Tals loaded successfully.\n";
}

void TaalManager::addTaal(const std::string &name, const std::vector<std::string> &bols) {
    tals[name] = Taal(name, bols);
}

std::optional<Taal> TaalManager::getTaal(const std::string &name) const {
    auto it = tals.find(name);
    if (it != tals.end()) {
        return it->second;
    }
    return std::nullopt; // Return empty optional if not found
}
