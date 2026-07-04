# DDKND: ECS-based Game Engine Runtime Prototype

[English](Readme.md) \| [日本語](Readme.ja.md)\|

## Portfolio Documents

The design intent and implementation details of this project are summarized in the following documents:

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

## structure
raw input -> Requested intent -> Resolver(FSM) -> ResolvedIntent -> UpdateLogic -> UpdateRendering

### IntentRequest
Creates a RequestedIntent from inputs, AI, Network, etc.

### State
Updates ActorState/FSM from RequestedIntent/Environment/Current State

### IntentResolve
Creates an Accepted Intent from RequestedIntent + State

### Logic
Updates Velocity/Gameplay state from Accepted Intent + Stats




## Problems


### Issues related to address instability caused by dynamic reallocation of AssetStorage::vector<T>, the container for AssetStore.
- The problem is that the address of the `Resource*`(T*) obtained with `AssetStore::TryGet(AssetID<T>)` depends on an internal container (such as `vector<T>`).
- Since addresses obtained with `TryGet()` are unstable, use them immediately after obtaining them as a temporary measure.
- In cases involving asynchronous loading or large-scale loading, a different structural solution is necessary.

## Old Implementation

The following repository contains an older implementation of this repository.

- https://github.com/MoryYamat/Dashbang_ECS_OLD