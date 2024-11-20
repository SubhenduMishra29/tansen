#include "TaalManager.h"
#include <fstream>
#include <iostream>
#include <stdexcept>

TaalManager::TaalManager() {}

void TaalManager::loadTaals(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open Taal data file.");
    }

    Json::Value root;
    file >> root;

    for (const auto& system : root.getMemberNames()) {
        for (const auto& taalName : root[system].getMemberNames()) {
            Taal taal;
            taal.name = taalName;
            taal.beats = root[system][taalName]["beats"].asInt();
            for (const auto& bol : root[system][taalName]["bols"]) {
                taal.bols.push_back(bol.asString());
            }
            taals[taal.name] = taal;
        }
    }
}

Taal TaalManager::getTaal(const std::string& name) const {
    if (taals.find(name) == taals.end()) {
        throw std::invalid_argument("Taal not found: " + name);
    }
    return taals.at(name);
}

void TaalManager::listAllTaals() const {
    for (const auto& [name, taal] : taals) {
        std::cout << name << " (" << taal.beats << " beats): ";
        for (const auto& bol : taal.bols) {
            std::cout << bol << " ";
        }
        std::cout << std::endl;
    }
}
