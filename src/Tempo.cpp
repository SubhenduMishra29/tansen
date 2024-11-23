#include "Tempo.h"

// Constructor: Initializes the Tempo object with a name and BPM value
Tempo::Tempo(const std::string& name, int bpm) : name(name), bpm(bpm) {}

// Get the name of the tempo
std::string Tempo::getName() const {
    return name;
}

// Get the beats per minute (BPM) of the tempo
int Tempo::getBPM() const {
    return bpm;
}
