# safenu

A Geode mod that adds a mod menu button to the main menu. It contains only safe, non-cheating options (currently: FPS counter).

## Build
1. Install the Geode SDK and set `GEODE_SDK`.
2. `geode build` (or `cmake -B build && cmake --build build`).
3. Change `yourname` in `mod.json` to your own developer name.

## Add options
Add a bool setting in `mod.json`, then call `addToggle(...)` in `SafenuPopup::setup()`.
