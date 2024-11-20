#ifndef MIDIHANDLER_H
#define MIDIHANDLER_H

#include "TaalManager.h"
#include "Tempo.h"
#include <string>

class MIDIHandler {
public:
    void generateTaalMIDI(const Taal& taal, const Tempo& tempo, const std::string& raag) const;
};

#endif
