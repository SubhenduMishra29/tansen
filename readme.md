RaagTaalProject/
├── include/
│   ├── TaalManager.h        # Manages all Taal definitions and retrieval.
│   ├── MIDIHandler.h        # Handles MIDI file generation logic.
│   ├── Tempo.h              # Defines tempo-related configurations.
│   ├── CommandParser.h      # Parses CLI commands.
├── src/
│   ├── TaalManager.cpp      # Implements TaalManager methods.
│   ├── MIDIHandler.cpp      # Implements MIDIHandler methods.
│   ├── Tempo.cpp            # Implements Tempo-related functions.
│   ├── CommandParser.cpp    # Implements command parsing logic.
│   ├── main.cpp             # Entry point of the application.
├── data/
│   └── taals.json           # JSON file storing all Hindustani, Carnatic, and Odiya Taal details.
├── output/
│   └── taal_track.mid       # Generated MIDI file for DAWs.
├── CMakeLists.txt           # Build system configuration.
└── README.md         
