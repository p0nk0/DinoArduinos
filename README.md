## Jurassic Park Booth - Arduino Code
This repo contains the code behind the four Arduino boards used our 2025 Spring Carnival attraction. See the [DinoGame](github.com/p0nk0/DinoGame) repo for more context.

## Overview
Each Arduino controlled a different hardware component and communicated with Unity Via serial.
 
## Arduinos
| Folder | Name | Role | Author | Libraries Needed |
|---|---|---|---|---|
| `/RFID` | DNA Scanner | Reads RFID tags | Taylor Roberts | MFRC522, SPI |
| `/PuzzleBox` | Circuit Breaker Puzzle | Reads switch inputs | Taylor Roberts | |
| `/dino_control` | Animatronic | Controlls dinosaur animatronic movements | Matthew Saenz | ServoEasing, MedianFilterLib |

## Setup
- All 

## Credits
