#ifndef TAAL_H
#define TAAL_H

#include <string>
#include <vector>
#include <map>
#include <optional>

class Taal {
public:
    Taal(const std::string &name, const std::vector<std::string> &bols)
        : name(name), bols(bols) {}

    const std::string &getName() const { return name; }
    const std::vector<std::string> &getBols() const { return bols; }

private:
    std::string name;                // Name of the Taal
    std::vector<std::string> bols;  // Sequence of bols (beats) in the Taal
};

class TaalManager {
public:
    // Loads predefined Tals (like Teentaal, Jhaptaal)
    void loadDefaultTals();

    // Add a custom Taal
    void addTaal(const std::string &name, const std::vector<std::string> &bols);

    // Get a Taal by name
    std::optional<Taal> getTaal(const std::string &name) const;

private:
    std::map<std::string, Taal> tals;  // Map of Taal name to Taal object
};

#endif // TAAL_H
