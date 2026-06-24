# Didakt Engine
###### Didaktos - That Which Can Be Taught
A 2D game engine built in C++ targeting indie-scale games. Supports sprite and tilemap rendering, a fixed-timestep game loop, and an entity component system. Lua scripting, Box2D physics, and audio are planned for later phases.

C++ · SDL2 · SDL_image · entt · GLM · nlohmann/json · CMake

---

## Build
Requires CMake 3.20+ and a C++17 compiler. Dependencies are pulled automatically via FetchContent.

```
cmake --preset x64-debug
cmake --build --preset x64-debug-build
```

Visual Studio support via Open As Folder. Set working directory to repo root.

---

## Status

| Phase | Topic | Status |
|-------|-------|--------|
| 1 | Project Core: engine window, renderer | ✅ |
| 2 | Game Loop: Frame and Fixed game loop with delta time | ✅ |
| 3 | 2D rendering: sprites, camera, tilemaps, layering | ✅ |
| 4 | Entity Component System: components, movement, render systems | ✅ |
| 5 | Input manager & event bus | 🔄 |
| 6 | Scene system |  |
| 7 | Scripting |  |
| 8 | Physics |  |
| 9 | Audio |  |
| 10 | Sample game |  |

---

## Design Notes

### Game Loop
Main game loop divided into two separate update branches, Frame and Fixed. Frame update runs unrestricted and handles input capture and rendering. Fixed update is locked to 60fps and handles systems that require consistency like physics and gameplay.

### System Architecture 
Game systems implemented as Entity Component System (ECS) via EnTT. Allows for efficient performance scaling and flexible game entity composition.

### Rendering
Rendering handled as part of the ECS. Allows any entity to be marked as renderable and sorted into the draw order via zIndex value. Camera struct enables world space positioning owned outside render system, enabling global access and multiple cameras support.
