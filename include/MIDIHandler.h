#ifndef MIDIHANDLER_H
#define MIDIHANDLER_H

#include "TaalManager.h"
#include "Tempo.h"
#include <string>

class MIDIHandler {
public:
    /**
     * @brief Generates a MIDI file representing the given Taal, Tempo, and Raag.
     *
     * @param taal The Taal structure containing rhythmic pattern and metadata.
     * @param tempo The Tempo object specifying BPM (beats per minute).
     * @param raag The name of the Raag (used for naming or metadata purposes).
     * @param outputPath The file path to save the generated MIDI file. Default is "output/taal_track.mid".
     */
    void generateTaalMIDI(const Taal& taal, const Tempo& tempo, const std::string& raag, const std::string& outputPath = "output/taal_track.mid") const;
};

#endif // MIDIHANDLER_H
