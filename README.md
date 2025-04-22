# QLink - A Tile-Matching Puzzle Game

## Project Overview

QLink is a tile-matching puzzle game developed using the Qt framework. Players control characters to move across the map and eliminate blocks of the same type. The game incorporates **RPG mechanics** and various tools, offering players a rich gaming experience.

## Basic Rules

Players can choose to start a new game in either **Single-Player Mode** or **Two-Player Mode**. They can also **load a saved game**, which will retain previous settings and progress. Within the time limit, players must connect two tiles with matching patterns using **a line with at most two bends** to score points. The game will automatically end if no removable tiles remain on the board.

## Features

- **Map Generation**: Randomly generates a game map of customizable size (MxN), with multiple types of blocks.
  
- **Character Control**: Players can freely control their characters to interact with tiles and trigger elimination.
  - *Single-Player Mode*: The character can be moved using `W`, `S`, `A`, `D` keys for up, down, left, and right movements.
  - *Two-Player Mode*: Player 1 uses `W`, `S`, `A`, `D` keys, and Player 2 uses arrow keys (`↑`, `↓`, `←`, `→`) for their respective character movements.

- **Tool System**: Random tools appear on the map, which players can automatically pick up and use when approached.
  - *Single-Player Mode*:
    - **Time Extension**: Adds 1 second to the game timer.
    - **Hint**: Highlights a pair of removable tiles for 10 seconds.
    - **Shuffle**: Rearranges the positions of all remaining tiles on the board.
    - **Teleportation**: Allows the player to control their character using a mouse for 5 seconds. Clicking on a block moves the character to it and selects it. Clicking on an empty space instantly teleports the character to that location.
  - *Two-Player Mode*:
    - **Time Extension**: Adds 1 second to the game timer.
    - **Hint**: Highlights a pair of removable tiles for 10 seconds.
    - **Shuffle**: Rearranges the positions of all remaining tiles on the board.
    - **Dizziness**: Reverses the other player’s controls for 10 seconds.
    - **Freeze**: Prevents the other player from moving for 3 seconds.

## Project Structure

```
├── README.md
├── QLink.pro
├── Headers                      // header files
│   ├── config.h                 // configuration file
│   ├── mainmenu.h
│   ├── block.h
│   ├── tool.h
│   ├── player.h
│   ├── singlegame.h
│   ├── doublegame.h
│   ├── singlegameovermenu.h
│   ├── doublegameovermenu.h
│   ├── usersinglesetting.h
│   └── userdoublesetting.h
├── Sources
│   ├── main.cpp
│   ├── mainmenu.cpp
│   ├── block.cpp
│   ├── tool.cpp
│   ├── player.cpp
│   ├── singlegame.cpp
│   ├── doublegame.cpp
│   ├── singlegameovermenu.cpp
│   ├── doublegameovermenu.cpp
│   ├── usersinglesetting.cpp
│   └── userdoublesetting.cpp
└── Resources
```
