# DDKND: ECS-based Game Engine Runtime Prototype

Readme.md languages  
| [English](Readme.md) | [日本語](Readme.ja.md) |

## Demo
[Demo video (YouTube)](https://youtu.be/XLh3T2ZbSMs)

### Features
- Rendering 100 or 1,000 animated characters
- Player character movement and attacks
- GPU skinning animation
- Visualization of hitboxes and debug information

## Build
This project can be configured and built using CMake Presets.

Debug build of the game application
```
cmake --preset x64-debug-game
cmake --build --preset x64-debug-game
```

Release build of the game application
```
cmake --preset x64-release-game
cmake --build --preset x64-release-game
```

Build and run ddknd engine tests
```
cmake --preset x64-debug-ddknd-tests
cmake --build --preset x64-debug-ddknd-tests
ctest --preset x64-debug-ddknd-tests
```

## Libraries Used
- fmt
- spdlog
- GLFW
- GLAD
- cgltf
- stb

## Tested environment
- Windows11
- C++20
- MSVC (cl.exe)
- CMake 3.25

## Portfolio Documents

The design intent and implementation details of this project are summarized in the following documents:


(All of these documents are in Japanese.)

**You can use this [reading guide](/docs/guides/reading_guide.md) to read these documents based on your own interests.**


- [Overview](docs/portfolio/overview.md)  
Provides an overview of the project's design and structure.
- [Engine Architecture](docs/portfolio/engine_architecture.md)  
Explains the runtime layer structure and the separation of Engine and App.
- [System Pipeline](docs/portfolio/system_pipeline.md)  
Explains the flow: input → intent → state → logic → rendering.
- [ECS](docs/portfolio/ecs.md)  
Explains the design of Entity, Registry, Storage, View, and Query.
- [Asset Manager](docs/portfolio/asset_manager.md)  
Explains the separation of AssetManager, AssetStorage, AssetStore, and Loader.
- [Import Pipeline](docs/portfolio/import_pipeline.md)  
Explains the separation of Importer, ImportData, and RuntimeResource.
- [Lessons Learned](docs/portfolio/lessons_learned.md)  
Explains challenges in the initial implementation and improvements made in the current implementation.
- [Roadmap](docs/portfolio/roadmap.md)  
Outlines current limitations and challenges, as well as future extensions.

### Appendix
- [Debug Tools](docs/portfolio/appendix_debug_tools.md)  
Explains the display of DebugDraw, DebugCamera, Skeleton, Hitbox/Hurtbox, and HitEvents.
- [FSM Engine](docs/portfolio/appendix_fsm_engine.md)  
Explains the design and implementation of a custom FSM engine (not used in the current implementation).
- [ECS-System Template Scheduler](docs/portfolio/appendix_ecs_template_scheduler.md)  
Explains a custom, non-intrusive library that resolves ECS system execution order at compile time.
- [Quality Checks and Improvements](docs/portfolio/appendix_quality.md)  
Describes the testing, static analysis, memory leak checks, and improvement strategies applied to the portfolio.

## Old Implementation

The following repository contains the previous implementation of this project.

- https://github.com/MoryYamat/Dashbang_ECS_OLD