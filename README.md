# 2048 Game Engine

A C++ implementation of the classic **2048 puzzle game**, developed with a focus on **Object-Oriented Programming (OOP)** concepts, matrix manipulation, game-state management, and clean modular design.

This project is being developed as part of an **Object-Oriented Programming course project**.

---

##  Project Overview

The **2048 Game Engine** recreates the core mechanics of the popular 2048 puzzle game.

The game is played on a **4 × 4 grid** containing numbered tiles. The player moves the tiles in four directions:

- Up
- Down
- Left
- Right

When two tiles containing the same value collide, they merge into a single tile whose value is the sum of the two tiles.

For example:

```text
2 + 2 = 4
4 + 4 = 8
8 + 8 = 16
```

After every valid move, a new tile is generated in an empty position. The objective is to continue merging tiles and reach the **2048 tile** while achieving the highest possible score.

---

## Objectives

The main objectives of this project are to:

- Implement the logic of the 2048 game using C++.
- Apply Object-Oriented Programming concepts in a practical project.
- Represent and manipulate the game board using a matrix.
- Implement tile movement and merging algorithms.
- Maintain the current game state and score.
- Detect winning and game-over conditions.
- Design the program using reusable and maintainable classes.
- Separate game logic from user interaction wherever possible.

---

##  Features

The planned game engine includes:

- 4 × 4 game board
- Random generation of new tiles
- Tile movement in four directions
- Automatic merging of equal-valued tiles
- Score calculation
- Game-state management
- Detection of valid and invalid moves
- Win-condition detection
- Game-over detection
- Board reset functionality
- Console-based board display
- Modular class-based architecture

---

##  Object-Oriented Programming Concepts

This project is designed to demonstrate important C++ OOP concepts such as:

### Classes and Objects

Different components of the game are represented using classes and objects to keep the implementation organized.

### Encapsulation

Internal game data such as the board, score, and current game state can be maintained as private data members and accessed through controlled member functions.

### Abstraction

The internal logic for shifting tiles, merging values, generating tiles, and checking game conditions is hidden behind simple game operations.

### Constructors

Constructors are used to initialize the board and other game-related objects.

### Inheritance and Polymorphism

Where appropriate, the project can be extended using inheritance and virtual functions for different game modes, input systems, or interfaces.

### Operator Overloading

Operator overloading may be incorporated where it provides a meaningful and clean way to manipulate game-related objects.

---

##  Proposed Class Structure

The project can be organized using classes such as:

```text
Game
│
├── Board
│   ├── Tile / Cell Management
│   ├── Move Operations
│   └── Merge Operations
│
├── Score Management
│
└── Game State
    ├── Running
    ├── Won
    └── Game Over
```

### `Board`

Responsible for:

- Maintaining the 4 × 4 matrix
- Moving tiles
- Merging matching tiles
- Finding empty cells
- Adding new tiles
- Checking whether moves are possible

### `Game`

Responsible for:

- Controlling the overall game
- Processing player moves
- Updating the score
- Managing the current game state
- Checking win and game-over conditions
- Restarting the game

Additional classes may be introduced as the project develops.

---

## Game Logic

For each player move, the game engine performs the following operations:

```text
Player Input
     ↓
Determine Direction
     ↓
Shift Tiles
     ↓
Merge Equal Tiles
     ↓
Shift Again
     ↓
Update Score
     ↓
Generate New Tile
     ↓
Check Game State
     ↓
Display Updated Board
```

For example:

```text
Before Left Move:

2   0   2   4
0   4   4   0
2   2   2   2
0   0   0   2

After Left Move:

4   4   0   0
8   0   0   0
4   4   0   0
2   0   0   0
```

---

## Controls

The console version can use the following controls:

| Key | Action |
|---|---|
| `W` | Move Up |
| `A` | Move Left |
| `S` | Move Down |
| `D` | Move Right |
| `R` | Restart Game |
| `Q` | Quit Game |

---

## Technologies Used

- **Language:** C++
- **Programming Paradigm:** Object-Oriented Programming
- **Data Structure:** 2D Matrix / Vector
- **Version Control:** Git
- **Repository Hosting:** GitHub

---

##  Project Structure

The project structure may evolve during development.

```text
2048-Game-Engine/
│
├── src/
│   ├── Game.cpp
│   └── Board.cpp
│
├── include/
│   ├── Game.h
│   └── Board.h
│
├── main.cpp
├── README.md
└── .gitignore
```

---
##  Compilation and Execution

### Using g++

Clone the repository:

```bash
git clone https://github.com/lathikaa-j/2048-Game-Engine.git
```

Navigate into the project directory:

```bash
cd 2048-Game-Engine
```

Compile the program:

```bash
g++ main.cpp src/Game.cpp src/Board.cpp -Iinclude -o game2048
```

Run the program:

### Windows

```bash
game2048.exe
```



> Compilation instructions may change as the project structure evolves.

---

## 🔄 Game States

The engine maintains different states during execution:

**Running** — The player can continue making moves.

**Won** — A tile with the value **2048** has been created.

**Game Over** — The board is full and no valid merges or movements are possible.

---

## Future Enhancements

Possible future improvements include:

- Undo functionality
- Save and load game
- High-score tracking
- Multiple board sizes
- Different difficulty levels
- Improved console interface
- Graphical User Interface (GUI)
- AI-based automatic player
- Move history
- Game statistics
- Custom winning tile values

---

## Learning Outcomes

Through this project, the following concepts are explored:

- C++ classes and objects
- Encapsulation and abstraction
- Constructors and destructors
- Inheritance and polymorphism
- Operator overloading
- Matrix manipulation
- STL containers
- Random number generation
- Game-state management
- Algorithm design
- Modular programming
- Git and GitHub version control

---

## Development Status

 **Project under development**

The project is being implemented incrementally, with the game engine, board operations, movement logic, and additional OOP features being added through subsequent commits.




 **2048 Game Engine — A C++ OOP implementation of the classic 2048 puzzle with matrix manipulation and game-state management.**