# **自作3Dゲーム~~エンジン~~**

## **開発環境**
* C++20
* CMake()

## ****



## **ゲーム基盤**

### **ECS**


### **Architecture Overview**
This game engine is based on a layered input-intent-resolution pipeline designed for maximum flexibility, testability, and separation of concerns.

```nginx
PlayerInput
   ↓
IntentMappingSystem
   ↓
IntentResolverSystem
   ↓
CharacterFSM
   ↓
GameLogicExecution
```

---

🔹 Purpose and Benefits
* Decoupling input devices from gameplay logic

* Unifying player, AI, and scripted behavior under a single "Intent" model

* Empowering modularity in skills, state transitions, and interaction resolution

---

🔸 Layer Details
1. Input → Intent
> Maps raw physical input (keys, controller) to high-level gameplay intents (e.g. "MoveForward", "UseSkill1")

* Allows flexible remapping and multi-device support

* Cleanly separates input from meaning

2. Intent → Resolver
> Resolves whether an intent can be accepted, based on current state (e.g. "Can't cast while stunned")

* Makes command validation explicit

* Clean point for cancel, interrupt, or rejection logic

3. Resolver → FSM
> Accepts validated intent and applies state transition (e.g. Idle → Casting)

* Based on orthogonal FSM, allowing concurrent and independent control domains

* Fully data-driven and reusable across actors

4. FSM → Logic
> Triggers gameplay consequences: animation, skill hitbox generation, cooldowns, etc.

* Logic is derived from state, not hardcoded

* Enables timing-based logic (e.g. ActiveTimeElapsed → Recovery)

---

🔹 Example Flow: Skill Casting
```pgsql
Player presses [Q]
→ Mapped to `Intent::CastSkill<SkillID>`
→ Resolver checks if current state allows casting
→ FSM transitions: None → Casting
→ Logic: Skill wind-up animation + damage hitbox created
→ After duration, FSM: Casting → Active → Recovery → None
```

---

### 🧠 Design Philosophy

This architecture reflects a belief that gameplay logic should be **state-driven, not input-driven**, and that actor behaviors should be unified regardless of source (player, AI, or script).  
It aims to maximize:

- **Modularity**: systems can evolve independently
- **Scalability**: logic grows without becoming entangled
- **Clarity**: debug and testing is simpler when intent is explicit

---  
### 🧩 Why This Matters

In traditional input systems, logic often becomes tightly coupled to control flow.  
This leads to problems such as:

- Hard-to-test behavior (input must be simulated)
- Unclear rejection rules (why did this action fail?)
- Inflexible control schemes (can’t rebind or re-use for AI)

By inserting `Intent` and `Resolver` layers, all behavior becomes explicit and state-aware.  
This is crucial for complex games with skills, cooldowns, interruptions, or AI decision making.

---

(Side note: this hierarchy reflects human behavior in the real world. Even though we want to do something, we can only act within the confines of our current state and environment. This led to a clear separation of intent, resolution, and execution in our systems.)

---
