# Font asset

Place a legally obtained, licensed TrueType font here as **2048.ttf**.
A readable font such as DejaVu Sans is suitable. Include its license when distributing it.
No system or external font has been copied or downloaded.

The default path assets/fonts/2048.ttf is relative to the working directory.
CMake copies assets beside game2048.exe. Run from the project root or executable
directory, or pass a font path as the first command-line argument.
Missing or invalid fonts produce an explicit error and exit code 1.
