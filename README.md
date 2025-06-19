# Snake Game Sample

This project is a simple snake game written using OpenGL, GLFW, and GLM. The game uses classic snake mechanics and is developed in C++.

---

## Contents

- [Installation](#installation)
- [Gameplay](#gameplay)
- [Tasks](#tasks)
- [Launch](#launch)
- [CMake Configuration](#cmake-configuration)

---

## Installation

### Requirements

- C++ compiler (GCC or Clang recommended)
- CMake (minimum version 2.8)
- GLFW 3.2 or higher
- GLM library
- GLAD (included in the project)
- A system supporting OpenGL

### Installation Steps

```bash
git clone https://github.com/username/snake-game.git
cd snake-game
mkdir build && cd build
cmake ..
make
./Game

## Gameplay

- Control the snake's movement direction using the arrow keys.  
- Press the `SPACE` key to add a new segment to the snake's tail.  
- The game ends if the snake moves out of the screen bounds.  
- Press the `ESC` key to exit the game.  
