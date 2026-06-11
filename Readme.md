
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