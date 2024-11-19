#include "taal.h"
#include <fstream>
#include <stdexcept>
#include <iomanip>

// Taal Class Implementation
Taal::Taal(const std::string& name, int beats, const std::vector<std::string>& bols)
    : name(name), beats(beats), bols(bols) {}

std::string Taal::getName() const {
    return name;
}

int Taal::getBeats() const {
    return beats;
}

std::vector<std::string> Taal::getBols() const {
    return bols;
}

// TaalManager Class Implementation
void TaalManager::addTaal(const std::string& name, const std::vector<std::string>& bols) {
    if (tals.find(name) != tals.end()) {
        throw std::runtime_error("Taal already exists: " + name);
    }
    tals[name] = Taal(name, bols.size(), bols);
}

void TaalManager::removeTaal(const std::string& name) {
    if (tals.erase(name) == 0) {
        throw std::runtime_error("Taal not found: " + name);
    }
}

Taal* TaalManager::getTaal(const std::string& name) {
    auto it = tals.find(name);
    return (it != tals.end()) ? &(it->second) : nullptr;
}

std::unordered_map<std::string, Taal> TaalManager::getAllTals() const {
    return tals;
}

void TaalManager::loadTalsFromJson(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open JSON file: " + filepath);
    }

    nlohmann::json jsonData;
    file >> jsonData;

    for (const auto& [category, taalList] : jsonData.items()) {
        for (const auto& taal : taalList) {
            addTaal(taal["name"], taal["beats"]);
        }
    }
}

void TaalManager::saveTalsToJson(const std::string& filepath) const {
    nlohmann::json jsonData;

    for (const auto& [name, taal] : tals) {
        jsonData["Custom"].push_back({
            {"name", taal.getName()},
            {"beats", taal.getBols()}
        });
    }

    std::ofstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to save JSON file: " + filepath);
    }

    file << std::setw(4) << jsonData; // Pretty print JSON
}
