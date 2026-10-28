# Phases 14-16: keyboard controls, gameplay UI and restart

## Implementation

- **Phase 14:** `InputHandler` translates SFML 2.6 key-pressed events into existing
  `Game` operations. Arrow keys and WASD move, U undoes, R starts a new game, and
  Escape requests closing the window. Unsupported keys and key releases are ignored.
  The event loop disables key repeat and tracks focus. A screen owner can pass
  `gameplayOwnsFocus = false`; paused screens also suppress gameplay shortcuts.
- **Phase 15:** The existing renderer now presents a score card, control hints,
  a New Game button, and translucent board overlays. Won displays "You reached
  2048!" with congratulations; Lost displays "Game Over!" with the final score.
  Both offer New Game and explain undo/restart/quit. Presentation reads the engine's
  state on every frame, so undo and restart immediately remove terminal overlays.
  Existing board geometry, tile palette, typography and 600 x 750 window are retained.
- **Phase 16:** Keyboard and mouse restart both call the already-correct
  `Game::newGame()`: clear board, score and undo history, restore Playing, spawn
  exactly two tiles. Buttons have hover feedback; only left clicks activate them.
  Terminal movement remains blocked by `Game::move()`, while U and R remain usable.

## Controls

| Key | Action |
| --- | --- |
| Arrow keys / W A S D | Move up, left, down, right respectively |
| U | Undo last valid move, including a winning/losing move |
| R | New game |
| Escape | Quit (there is no connected menu) |
| Left click New Game | New game |

## Files

Created:
- `include/ui/InputHandler.h`
- `src/ui/InputHandler.cpp`
- `include/ui/Button.h`
- `tests/InputRegression.cpp`
- `PHASES_14_16.md`

Modified:
- `main.cpp`: mutable game, keyboard dispatch, focus tracking and restart clicks.
- `include/ui/Renderer.h`, `src/ui/Renderer.cpp`: score card, buttons, hints, overlays.
- `include/core/Game.h`, `src/core/Game.cpp`: additive snapshot constructor.
- `CMakeLists.txt`: shared UI library, GUI regression target, explicit include path.

## Design and teammate integration

InputHandler -> Game -> Board keeps movement, merging, spawning and scoring in the
existing engine. Renderer accepts const game references. Button owns its geometry
and text and borrows the renderer's font only during drawing. Existing Board
operators, History template, exceptions, RAII and public methods remain intact.

The only engine API addition is `explicit Game(const Board&, int initialScore = 0)`.
It copies a board snapshot, validates a nonnegative score, derives state through
the existing updateState routine and starts with empty history. Tests use this
to exercise terminal situations without mutable access to Game's board or random
searches. Default construction and newGame retain their existing behavior.

Reused the existing Game movement/score/undo/state/restart implementation and
Renderer board/tile drawing. The inspected branch contains teammate classes for
profiles, modes, persistence, statistics and leaderboards, but neither main nor
the original CMake engine target connects them to gameplay. They are untouched.
There is no selected mode in Game, connected menu, best-score getter/persistence
service, or continue-beyond-2048 rule. Consequently this phase adds no best-score
system, mode changes, menu replacement or Continue Playing option. Connecting
those standalone modules remains separate integration work.

## Build, test and run (PowerShell, repository root)

```powershell
$env:Path = "C:\mingw13\mingw64\bin;" + $env:Path
cmake -S . -B build -G "MinGW Makefiles" -DGAME2048_BUILD_GUI=ON -DSFML_DIR="C:/SFML/SFML-2.6.2/lib/cmake/SFML"
cmake --build build
ctest --test-dir build --output-on-failure
.\build\game2048.exe
```

Assets are copied beside the executable; launch from the repository root or build
directory, or provide the font path as the first argument. GUI-free builds still
use `-DGAME2048_BUILD_GUI=OFF`; the input suite is conditional on GUI and testing.
Both `build/` and `build-backend/` remain ignored.

The existing build directory already contains SFML runtime DLLs. For a fresh
directory, place `sfml-graphics-2.dll`, `sfml-window-2.dll`, and `sfml-system-2.dll`
from `C:\SFML\SFML-2.6.2\bin` beside both GUI and input-test executables (use
the corresponding `-d-2.dll` variants for a debug build). Keep the MinGW bin
directory on PATH for compiler runtime DLLs.

## Verification results and limitations

Verified on October 17, 2026 with MinGW GCC and SFML 2.6.2:
- CMake GUI configuration and complete build succeeded; no compiler warnings emitted.
- CTest: 2/2 suites passed.
- Existing `engine_regression`: 84 checks passed, zero failures; source unchanged.
- New `input_regression`: 69 checks passed, zero failures. Covers all eight movement
  mappings against identical copied RNG state, single spawn and scoring, blocked
  input, undo, key releases, unsupported keys, screen/focus ownership, paused
  input, Escape, controlled winning merge, controlled last slide into game over,
  terminal movement blocking, restart after won/lost, 30 restarts after undo,
  and header/overlay button hit targets including paused and right-click rejection.

The sandbox generated empty compiler include paths; the same CMake build succeeded
outside the sandbox. No tracked build artifacts were added. No branch switches,
merges, staging, commits or pushes were performed; work stays on `lathikaa`.

The GUI was not launched or visually inspected: the desktop computer-use helper
reported an unavailable native pipe. Manual checks remain for text clipping,
overlay appearance, hover feedback, OS focus changes and holding a physical key.
Repeat suppression is implemented using SFML's `setKeyRepeatEnabled(false)`.
