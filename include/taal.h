#ifndef TAAL_H
#define TAAL_H

#include <string>
#include <vector>
#include <unordered_map>
#include <nlohmann/json.hpp>

class Taal {
public:
    Taal(const std::string& name, int beats, const std::vector<std::string>& bols);

    std::string getName() const;
    int getBeats() const;
    std::vector<std::string> getBols() const;

private:
    std::string name;
    int beats;
    std::vector<std::string> bols;
};

class TaalManager {
public:
    void addTaal(const std::string& name, const std::vector<std::string>& bols);
    void removeTaal(const std::string& name);
    Taal* getTaal(const std::string& name);
    std::unordered_map<std::string, Taal> getAllTals() const;

    // JSON file handling
    void loadTalsFromJson(const std::string& filepath);
    void saveTalsToJson(const std::string& filepath) const;

private:
    std::unordered_map<std::string, Taal> tals; // Store all Tals by name
};

#endif // TAAL_H
