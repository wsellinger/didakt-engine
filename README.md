# Didakt Engine
###### Didaktos - That Which Can Be Taught
A 2D game engine built in C++ targeting indie-scale games. Supports sprite and tilemap rendering, a fixed-timestep game loop, and an entity component system. Input, logging, timing, windowing, and rendering are abstracted behind swappable provider interfaces, with SDL2 as the current backend. Lua scripting, Box2D physics, and audio are planned for later phases.

C++ · SDL2 · SDL_image · entt · GLM · nlohmann/json · Catch2 · CMake

---

## Build
Requires CMake 3.20+ and a C++17 compiler. Dependencies are pulled automatically via FetchContent.

```
cmake --preset x64-debug
cmake --build --preset x64-debug-build
```

Visual Studio support via Open As Folder. Set working directory to repo root.

Tests run via CTest:
```
ctest --test-dir out/build/x64-debug
```

---

## Status

| Phase | Topic | Status |
|-------|-------|--------|
| 1 | Project Core: engine window, renderer | ✅ |
| 2 | Game Loop: Frame and Fixed game loop with delta time | ✅ |
| 3 | 2D rendering: sprites, camera, tilemaps, layering | ✅ |
| 4 | Entity Component System: components, movement, render systems | ✅ |
| 5 | Input manager, event bus | ✅ |
| 5.5 | Test suite, dependency injection | ✅ |
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

### Implementation Abstraction
Subsystems touching external libraries defined as an interfaces to abstract implementation from engine infrastructure. `ProviderManager` owns construction, initialization order, and teardown of all providers; `ProviderManagerFactory` selects the concrete backend at compile time behind a single `DIDAKT_USE_SDL` definition. Implementations not suitable for interfaces (like asserts) are implemented as macros wrapping the concrete implementation. This unlocks testability for engine infrastructure and allows for Strangler Fig migration path for future, more customized implementations.

### Event System
`EventBus` provides type-safe pub/sub for decoupling systems. Implemented via internally type-erased callbacks behind `std::type_index`, so `Subscribe<T>`/`Emit<T>` remain fully typed at the call site. Supports unsubscribe via an opaque `SubscriptionId`. 

### Testing
Catch2-based unit test suite (`DidaktTests` target) covers pure-logic components. Provider interfaces allow mock implementations for replacing concrete dependencies.