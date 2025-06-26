## Jurassic Park Booth - Arduino Code
This repo contains the code for the three Arduinos used in our 2025 Spring Carnival attraction. For more context, see the [DinoGame](github.com/p0nk0/DinoGame) repo.

## Overview
Each Arduino controlled a different hardware component and communicated with Unity via serial.
 
## Arduinos
| Folder | Name | Role | Author | Libraries Needed | Arduino Type |
|---|---|---|---|---|---|
| `/RFID` | DNA Scanner | Reads RFID tags | Taylor Roberts | MFRC522, SPI | MKR Zero |
| `/PuzzleBox` | Circuit Breaker Puzzle | Reads switch inputs | Taylor Roberts | | Uno |
| `/dino_control` | Animatronic | Controls dinosaur animatronic movements | Matthew Saenz | ServoEasing, MedianFilterLib | Mega |

## Setup
- See `/Libraries` for the needed non-standard libraries
- In Unity, make sure each `SerialController` object uses the correct baud rate and COM port for its corresponding Arduino.

## Media
DNA Scanner:

![DNA Scanner](Images/RFID.jpg)

Circuit Breaker Puzzle:

![Puzzle Box](Images/PuzzleBox.png)

Animatronic Dinosaur:

![Animatronic](Images/Dino.jpg)
