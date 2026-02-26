#include <iostream>
#include <map>
#include <vector>
#include <string>

class Taal {
public:
    std::string name;
    int beats;
    std::vector<std::string> bols;

    Taal(const std::string& name, int beats, const std::vector<std::string>& bols)
        : name(name), beats(beats), bols(bols) {}
};

class TaalSystem {
private:
    std::map<std::string, Taal> tals; // Map to store all tals

public:
    TaalSystem() {
        // Adding Carnatic tals
        tals["Adhi Taal"] = Taal("Adhi Taal", 8, {"Dha", "Dhin", "Na", "Dhin", "Dha", "Tin", "Na", "Dhin"});
        tals["Rupaka Taal"] = Taal("Rupaka Taal", 6, {"Dha", "Tin", "Na", "Dhin", "Na", "Dha"});
        tals["Tisra Taal"] = Taal("Tisra Taal", 10, {"Dha", "Dhin", "Na", "Dhin", "Na", "Dha", "Tin", "Na", "Dhin", "Na"});
        tals["Chapu Taal"] = Taal("Chapu Taal", 7, {"Dha", "Tin", "Na", "Dhin", "Na", "Tin", "Dha"});
        tals["Khanda Taal"] = Taal("Khanda Taal", 5, {"Dha", "Dhin", "Na", "Tin", "Na"});
        tals["Misra Taal"] = Taal("Misra Taal", 7, {"Dha", "Tin", "Na", "Dhin", "Na", "Tin", "Dha"});

        // Adding Hindustani tals
        tals["Teentaal"] = Taal("Teentaal", 16, {"Dha", "Dhin", "Dhin", "Dha", "Dhin", "Na", "Dhin", "Dha", "Tin", "Na", "Dhin", "Dhin", "Dha", "Na", "Dhin", "Dha"});
        tals["Ektaal"] = Taal("Ektaal", 12, {"Dha", "Dhin", "Na", "Dhin", "Na", "Dha", "Tin", "Na", "Dhin", "Na", "Dhin", "Dha"});
        tals["Jhaptaal"] = Taal("Jhaptaal", 10, {"Dha", "Tin", "Na", "Dhin", "Dhin", "Na", "Dha", "Dhin", "Na", "Dha"});
        tals["Ada Chautal"] = Taal("Ada Chautal", 14, {"Dha", "Dhin", "Na", "Dhin", "Na", "Dha", "Tin", "Na", "Dhin", "Dha", "Na", "Dhin", "Na", "Dha"});
        tals["Dhamar"] = Taal("Dhamar", 14, {"Dha", "Dhin", "Na", "Dhin", "Na", "Dha", "Tin", "Na", "Dhin", "Dha", "Na", "Dhin", "Na", "Dha"});
        tals["Keherwa"] = Taal("Keherwa", 8, {"Dha", "Dhin", "Dhin", "Dha", "Dha", "Dhin", "Dhin", "Dha"});
        tals["Chautal"] = Taal("Chautal", 12, {"Dha", "Dhin", "Na", "Dhin", "Na", "Dha", "Tin", "Na", "Dhin", "Dha", "Na", "Dhin"});

        // Adding Odissi tals
        tals["Trisra Jati"] = Taal("Trisra Jati", 7, {"Dha", "Dhin", "Na", "Dhin", "Dha", "Tin", "Na"});
        tals["Ektali"] = Taal("Ektali", 4, {"Dha", "Dhin", "Na", "Dhin"});
        tals["Rupaka Taal"] = Taal("Rupaka Taal", 6, {"Dha", "Tin", "Na", "Dhin", "Na", "Dha"});
        tals["Bhairavi Taal"] = Taal("Bhairavi Taal", 16, {"Dha", "Dhin", "Na", "Dhin", "Na", "Dha", "Tin", "Na", "Dhin", "Dha", "Na", "Dhin", "Na", "Dha", "Dhin", "Dha"});
        tals["Dhruva Taal"] = Taal("Dhruva Taal", 12, {"Dha", "Dhin", "Na", "Dhin", "Na", "Dha", "Tin", "Na", "Dhin", "Dha", "Na", "Dhin"});
    }

    // Add a new taal dynamically
    void addTaal(const std::string& name, int beats, const std::vector<std::string>& bols) {
        if (tals.find(name) != tals.end()) {
            std::cerr << "Taal already exists: " << name << std::endl;
            return;
        }
        tals[name] = Taal(name, beats, bols);
        std::cout << "Added new Taal: " << name << std::endl;
    }

    // Display all available tals
    void displayTals() const {
        for (const auto& pair : tals) {
            std::cout << pair.second.name << " (" << pair.second.beats << " beats): ";
            for (const auto& bol : pair.second.bols) {
                std::cout << bol << " ";
            }
            std::cout << std::endl;
        }
    }

    // Get the bols for a specific taal
    std::vector<std::string> getBols(const std::string& name) const {
        if (tals.find(name) == tals.end()) {
            std::cerr << "Taal not found: " << name << std::endl;
            return {};
        }
        return tals.at(name).bols;
    }
};

// Example usage
int main() {
    TaalSystem taalSystem;

    // Display default tals
    std::cout << "Default Tals:" << std::endl;
    taalSystem.displayTals();

    // Add a new taal dynamically
    taalSystem.addTaal("Custom Taal", 6, {"Dha", "Na", "Tin", "Na", "Dha", "Tin"});

    // Display updated tals
    std::cout << "\nUpdated Tals:" << std::endl;
    taalSystem.displayTals();

    return 0;
}
