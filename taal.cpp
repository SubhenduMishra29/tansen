#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <stdexcept>

class Taal {
public:
    std::string name;
    int beats;
    std::vector<std::string> boles;
    std::string time_signature;
    std::vector<std::string> variations;

    // Constructor for Taal
    Taal(const std::string& name, int beats, const std::vector<std::string>& boles, const std::string& time_signature = "4/4")
        : name(name), beats(beats), boles(boles), time_signature(time_signature) {}

    // Function to display the Taal details
    void display() const {
        std::cout << "Taal: " << name << ", Beats: " << beats << ", Time Signature: " << time_signature << " | Boles: ";
        for (const auto& bole : boles) {
            std::cout << bole << " ";
        }
        std::cout << "\nVariations: ";
        for (const auto& var : variations) {
            std::cout << var << " ";
        }
        std::cout << std::endl;
    }
};

class TaalManager {
private:
    std::unordered_map<std::string, Taal> tals;

public:
    TaalManager() {
        // Adding Carnatic Tals
        tals["Adhi Taal"] = Taal("Adhi Taal", 8, {"Dha", "Dhin", "Na", "Dhin", "Dha", "Tin", "Na", "Dhin"});
        tals["Rupaka Taal"] = Taal("Rupaka Taal", 6, {"Dha", "Tin", "Na", "Dhin", "Na", "Dha"});
        tals["Tisra Taal"] = Taal("Tisra Taal", 10, {"Dha", "Dhin", "Na", "Dhin", "Na", "Dha", "Tin", "Na", "Dhin", "Na"});
        tals["Chapu Taal"] = Taal("Chapu Taal", 7, {"Dha", "Tin", "Na", "Dhin", "Na", "Tin", "Dha"});
        tals["Khanda Taal"] = Taal("Khanda Taal", 5, {"Dha", "Dhin", "Na", "Tin", "Na"});
        tals["Misra Taal"] = Taal("Misra Taal", 7, {"Dha", "Tin", "Na", "Dhin", "Na", "Tin", "Dha"});

        // Adding Hindustani Tals
        tals["Teentaal"] = Taal("Teentaal", 16, {"Dha", "Dhin", "Dhin", "Dha", "Dhin", "Na", "Dhin", "Dha", "Tin", "Na", "Dhin", "Dhin", "Dha", "Na", "Dhin", "Dha"});
        tals["Ektaal"] = Taal("Ektaal", 12, {"Dha", "Dhin", "Na", "Dhin", "Na", "Dha", "Tin", "Na", "Dhin", "Na", "Dhin", "Dha"});
        tals["Jhaptaal"] = Taal("Jhaptaal", 10, {"Dha", "Tin", "Na", "Dhin", "Dhin", "Na", "Dha", "Dhin", "Na", "Dha"});
        tals["Ada Chautal"] = Taal("Ada Chautal", 14, {"Dha", "Dhin", "Na", "Dhin", "Na", "Dha", "Tin", "Na", "Dhin", "Dha", "Na", "Dhin", "Na", "Dha"});
        tals["Dhamar"] = Taal("Dhamar", 14, {"Dha", "Dhin", "Na", "Dhin", "Na", "Dha", "Tin", "Na", "Dhin", "Dha", "Na", "Dhin", "Na", "Dha"});
        tals["Keherwa"] = Taal("Keherwa", 8, {"Dha", "Dhin", "Dhin", "Dha", "Dha", "Dhin", "Dhin", "Dha"});
        tals["Chautal"] = Taal("Chautal", 12, {"Dha", "Dhin", "Na", "Dhin", "Na", "Dha", "Tin", "Na", "Dhin", "Dha", "Na", "Dhin"});
        tals["Mishra Chapu"] = Taal("Mishra Chapu", 7, {"Dha", "Dhin", "Na", "Dhin", "Na", "Dhin", "Na"});
        tals["Tisra Taal"] = Taal("Tisra Taal", 10, {"Dha", "Dhin", "Dhin", "Dha", "Tin", "Na", "Dhin", "Dhin", "Dha", "Na"});
        tals["Dadra"] = Taal("Dadra", 6, {"Dha", "Dhin", "Dhin", "Dha", "Dhin", "Dha"});

        // Adding Odissi Tals
        tals["Trisra Jati"] = Taal("Trisra Jati", 7, {"Dha", "Dhin", "Na", "Dhin", "Dha", "Tin", "Na"});
        tals["Ektali"] = Taal("Ektali", 4, {"Dha", "Dhin", "Na", "Dhin"});
        tals["Rupaka Taal"] = Taal("Rupaka Taal", 6, {"Dha", "Tin", "Na", "Dhin", "Na", "Dha"});
        tals["Jhaptaal"] = Taal("Jhaptaal", 10, {"Dha", "Tin", "Na", "Dhin", "Dhin", "Na", "Dha", "Dhin", "Na", "Dha"});
        tals["Choutala"] = Taal("Choutala", 12, {"Dha", "Dhin", "Na", "Dhin", "Na", "Dha", "Tin", "Na", "Dhin", "Dha", "Na", "Dhin"});
        tals["Mishra Jhaptaal"] = Taal("Mishra Jhaptaal", 10, {"Dha", "Tin", "Na", "Dhin", "Na", "Dhin", "Na", "Dha", "Na", "Dha"});
        tals["Bhairavi Taal"] = Taal("Bhairavi Taal", 16, {"Dha", "Dhin", "Na", "Dhin", "Na", "Dha", "Tin", "Na", "Dhin", "Dha", "Na", "Dhin", "Na", "Dha", "Dhin", "Dha"});
        tals["Sooltaal"] = Taal("Sooltaal", 10, {"Dha", "Tin", "Na", "Dhin", "Na", "Dhin", "Na", "Dha", "Na", "Dha"});
        tals["Dhruva Taal"] = Taal("Dhruva Taal", 12, {"Dha", "Dhin", "Na", "Dhin", "Na", "Dha", "Tin", "Na", "Dhin", "Dha", "Na", "Dhin"});

        // Adding variations (Dogun, Teen Gun, Chauguni, etc.)
        tals["Teentaal"].variations.push_back("Dogun");
        tals["Teentaal"].variations.push_back("Chauguni");
        tals["Teentaal"].variations.push_back("Teen Gun");
        tals["Keherwa"].variations.push_back("Dogun");
    }

    // Function to add a new Taal
    void addTaal(const Taal& newTaal) {
        if (tals.find(newTaal.name) != tals.end()) {
            throw std::invalid_argument("Taal already exists.");
        }
        tals[newTaal.name] = newTaal;
    }

    // Function to get a Taal by name
    Taal getTaal(const std::string& name) const {
        auto it = tals.find(name);
        if (it == tals.end()) {
            throw std::invalid_argument("Taal not found.");
        }
        return it->second;
    }

    // Function to list all Tals
    void listAllTals() const {
        for (const auto& entry : tals) {
            entry.second.display();
        }
    }
};

int main() {
    TaalManager manager;
    manager.listAllTals();

    // Adding a new taal dynamically
    Taal newTaal("New Taal", 8, {"Dha", "Tin", "Na", "Dhin", "Na", "Dha", "Tin", "Na"});
    manager.addTaal(newTaal);

    // Listing all tals after adding the new one
    std::cout << "\nAfter adding new taal:\n";
    manager.listAllTals();

    return 0;
}
