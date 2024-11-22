#include "TaalManager.h"
#include <fstream>
#include <iostream>
#include <stdexcept>

// Constructor
TaalManager::TaalManager() {}

// Load Taals from JSON file
void TaalManager::loadTaals(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open Taal data file: " + filePath);
    }

    Json::Value root;
    try {
        file >> root;
    } catch (const std::exception& e) {
        throw std::runtime_error("Error parsing JSON: " + std::string(e.what()));
    }

    for (const auto& system : root.getMemberNames()) {
        const Json::Value& systemData = root[system];
        for (const auto& taalName : systemData.getMemberNames()) {
            const Json::Value& taalData = systemData[taalName];

            // Validate required fields
            if (!taalData.isMember("beats") || !taalData.isMember("bols")) {
                throw std::runtime_error("Malformed Taal data: Missing 'beats' or 'bols' for Taal: " + taalName);
            }

            Taal taal;
            taal.name = taalName;
            taal.beats = taalData["beats"].asInt();
            for (const auto& bol : taalData["bols"]) {
                taal.bols.push_back(bol.asString());
            }
            taals[taal.name] = taal;
        }
    }
}

// Get a specific Taal by name
Taal TaalManager::getTaal(const std::string& name) const {
    auto it = taals.find(name);
    if (it == taals.end()) {
        throw std::invalid_argument("Taal not found: " + name);
    }
    return it->second;
}

// List all Taals
void TaalManager::listAllTaals() const {
    for (const auto& [name, taal] : taals) {
        std::cout << name << " (" << taal.beats << " beats): ";
        for (const auto& bol : taal.bols) {
            std::cout << bol << " ";
        }
        std::cout << std::endl;
    }
}
