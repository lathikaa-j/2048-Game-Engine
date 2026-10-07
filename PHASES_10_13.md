# Phases 10–13

The project remains C++ only. Keyboard gameplay controls and later phases are not implemented.
The regression harness formerly in main.cpp is preserved in tests/Regression.cpp.

## Verified locally

MinGW.org GCC 6.3.0 was detected. Backend build succeeded: 84 checks passed,
0 failed; CTest passed 1/1 regression target. SFML was not found by CMake.
The graphical application has not compiled, launched, or been visually inspected.
No font asset was present or downloaded.

## Dependency setup

Frontend targets the SFML 2.6 API and links only graphics, window, system.
SFML 3 is not supported. The official SFML 2.6.2 Windows MinGW binaries use
GCC 13.1.0: do not link these to GCC 6.3.0. Match compiler version, architecture,
threading model, runtime, and exception model. No compiler has been changed.

To retain the current compiler, obtain SFML 2.6.2 source from the official download
page and build it with exactly the same MinGW compiler used for this project.
This dependency build has not been attempted or verified here. In a PowerShell
terminal where the existing MinGW compiler and mingw32-make are on PATH:

~~~powershell
# Replace these paths with your extracted source and chosen install directory.
cmake -S <sfml-source> -B <sfml-build> -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=<sfml-install> -DSFML_BUILD_AUDIO=OFF -DSFML_BUILD_NETWORK=OFF -DSFML_BUILD_EXAMPLES=OFF -DSFML_BUILD_DOC=OFF
cmake --build <sfml-build>
cmake --install <sfml-build>
~~~

Ensure CMake selects the same compiler as the game. If dependency compilation fails
with the old toolchain, stop and resolve compatibility before integrating binaries.
An alternative is a deliberate compiler upgrade paired with its exact official SFML
binary package; this requires your own toolchain decision and a fresh build directory.
See https://www.sfml-dev.org/download/sfml/2.6.2/ and
https://www.sfml-dev.org/tutorials/2.6/getting-started/build-from-source/ .

## Build and run

Add a licensed TrueType font as assets/fonts/2048.ttf, including its license when
distributing it. See assets/fonts/README.md. From the project root:

~~~powershell
cmake -S . -B build -G "MinGW Makefiles" -DGAME2048_BUILD_GUI=ON -DSFML_DIR=<sfml-install>/lib/cmake/SFML
cmake --build build
ctest --test-dir build --output-on-failure
.\build\game2048.exe
~~~

Reconfigure the pre-existing build directory because CMake targets have changed.
For dynamic SFML, place the matching SFML graphics/window/system DLLs and required
runtime/dependency DLLs beside game2048.exe or add the matching package bin directory
to this terminal's PATH. Do not mix DLLs from another compiler/package.
CMake copies project assets beside the executable; running from the project root
also finds the default asset path. An optional first argument specifies a font path.

Without SFML, backend tests remain available:

~~~powershell
cmake -S . -B build-backend -G "MinGW Makefiles" -DGAME2048_BUILD_GUI=OFF
cmake --build build-backend
ctest --test-dir build-backend --output-on-failure
.\build-backend\engine_regression.exe
~~~

If mingw32-make is not on PATH, set CMAKE_MAKE_PROGRAM to your existing make executable
when configuring a fresh directory. The local backend build reused the make executable
identified in the pre-existing CMake cache.

## Manual graphical checklist

Check the 600 by 750 window title, responsiveness, and close button. Check the calm
background, centered 500-pixel board, 16 equally spaced cells, exactly two initial
2/4 tiles with centered readable numbers, blank empty cells, Score: 0, and Playing.
Keyboard movement is intentionally absent. Higher-value tile colors and smaller
number sizes are implemented but not exercised by the initial random board.
Renderer takes const Game/Board references and maintains no separate board state.
Missing/invalid font should print a useful error and exit with status 1.

## Design

Invalid Direction enum values are caller errors and throw InvalidMoveException.
Ordinary blocked moves still return false. Tests catch the derived type and the base
GameException, demonstrating inheritance and virtual what() dispatch.
A missing font is a setup error reported with GameException at application startup.
Renderer calculates positions and draws rectangles/text only. Game retains all
movement, scoring, state, spawning, and undo responsibilities.
