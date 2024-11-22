#ifndef TEMPO_H
#define TEMPO_H

#include <string>

class Tempo {
private:
    std::string name; // Name of the tempo (e.g., Bilambit, Madhya, Drut)
    int bpm;          // Beats per minute (tempo value)

public:
    /**
     * @brief Constructor for the Tempo class.
     * 
     * @param name The name of the tempo (e.g., "Bilambit", "Madhya", "Drut").
     * @param bpm The tempo value in beats per minute.
     */
    Tempo(const std::string& name, int bpm);

    /**
     * @brief Get the name of the tempo.
     * 
     * @return The name of the tempo as a string.
     */
    std::string getName() const;

    /**
     * @brief Get the BPM (beats per minute) of the tempo.
     * 
     * @return The BPM value as an integer.
     */
    int getBPM() const;
};

#endif // TEMPO_H
