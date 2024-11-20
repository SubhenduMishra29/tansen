# Tansen

**Tansen** is a CLI-based tool for generating **MIDI tracks** based on Indian classical music principles. It supports a wide variety of **Hindustani**, **Carnatic**, and **Odiya Taal**, and allows users to dynamically generate rhythm tracks with customizable Raag, Taal, and Tempo for use in DAWs like Fruity Loops or Ableton Live.

---

## **Features**
- Comprehensive library of Hindustani, Carnatic, and Odiya Taals.
- Dynamic tempo control (Vilambit, Madhya, Drut).
- Generate rhythmic MIDI tracks for use in professional DAWs.
- Support for creating and modifying custom Taals.
- Command-based user interaction.

---

## **Project Structure**

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

## **Installation**

### **Dependencies**
- A C++17 compiler (e.g., GCC or Clang)
- CMake (version 3.10 or higher)
- [nlohmann-json](https://github.com/nlohmann/json) library for JSON parsing
- DAW software to use the generated MIDI files (e.g., Fruity Loops, Ableton Live)

### **Build Instructions**
1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/tansen.git
   cd tansen
2. Create a build directory and configure the project:
    ```bash
   mkdir build
   cd build
   cmake ..
3. Build the executable:
    ```bash
  
   make
4. Run the application:
    ```bash
    ./bin/Tansen
    
## **Usage**
### **Commands**

1. Create a Taal Track
 ```bash
Raag <raag_name> Taal <taal_name> Tempo <tempo_name>

### **Example:**

 ```bash
Raag Bhairavi Taal Keherva Tempo Vilambit
Generates a MIDI track for Raag Bhairavi with Keherva Taal at Vilambit speed.

2. List Available Taals

 ```bash
Copy code
list taals
3. Add Custom Taal

 ```bash

add taal <taal_name> <beats> <bol1> <bol2> ... <bolN>
### **Example:**

 ```bash

add taal MyTaal 8 Dha Ge Na Ti Na Ke Tin Na
4. Set Tempo

 ```bash

set tempo <tempo_name>
## Output
The generated MIDI file is stored in the output/ directory as taal_track.mid.
Supported Taals
Hindustani Taals
Teentaal
Jhaptaal
Ektaal
Keherva
Dadra
Roopak
Jhoola
Khemta
Deepchandi
Carnatic Taals
Adi
Rupaka
Triputa
Jhampa
Dhruva
Ata
Odiya Taals
Jhampa
Addha
Khemta
Loka
License
This project is licensed under the MIT License. See the LICENSE file for details.

Contributing
Fork the repository.
Create a new branch (git checkout -b feature-name).
Commit your changes (git commit -am 'Add some feature').
Push to the branch (git push origin feature-name).
Create a pull request.
Contact
For questions, suggestions, or contributions, please reach out to:

Email: your-email@example.com
GitHub: your-username
Enjoy creating music with Tansen! 🎵






