
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