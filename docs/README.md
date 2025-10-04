# Chess Game (C++ with SDL)

## 📌 Overview
This is a chess game implemented in **C++**, using **SDL2** for graphics rendering. 

## 🔧 Build Instructions
### Requirements
- A C++17 compatible compiler (g++/clang++)  
- [SDL2]  
- [SDL2_image]  

### Linux / macOS
```bash
g++ src/*.cpp -lSDL2 -lSDL2_image -o main
./main

### Windows 
g++ src/*.cpp -IC:/path/to/SDL2/include -LC:/path/to/SDL2/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -o main.exe
main.exe