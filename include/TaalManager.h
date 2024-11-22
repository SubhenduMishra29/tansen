#ifndef TAALMANAGER_H
#define TAALMANAGER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <json/json.h>

// Taal structure
struct Taal {
    std::string name;
    int beats;
    std::vector<std::string> bols;
};

class TaalManager {
private:
    std::unordered_map<std::string, Taal> taals;

public:
    TaalManager();
    void loadTaals(const std::string& filePath);
    Taal getTaal(const std::string& name) const;
    void listAllTaals() const;
};

#endif // TAALMANAGER_H
