# **Identifying issues in the development process and consolidating solutions**

## **Project**
* Convert the local character code from `Shift-JIS` to `UTF-8` in bulk
* **Organizing the folder structure**

## **Architecture
* Separation of game logic and system (engine) logic

## **ECS**
* Defining hierarchical relationships between `Entity` and implementing semantics based on names
    * Processing to limit duplicate `Entity` names
    * Name search processing -> Improved efficiency
* *** Consider a good way to manage the lifespan of `Entity` (even if you delete an `Entity`, the `Component` will remain. If there is a process that references the `Entity`, you need to manage the deletion carefully)**


## **Physical Layer**
* Physics layer (collision) implementation
    * Ground implementation
* Implement flexible and intuitive integration of drawing data and physics data
    * Consider how to match the apparent size with meaningful collision detection/action range in logical space (logic) and visual space (visual).

## **Camera**
* Separation of system camera and game camera
* Implementing a logical camera (taking pictures from directly above the XZ plane)

## **input**
* Strict separation of key state management and side effects caused by state changes

## **RenderSystem**
* ~~Accurately extracting OpenGL buffers such as VAO and VBO from `MeshComponent` and coordinating `ModelData`~~(2025/04/26)
    * The solution is to add a for loop and handle intermediate data.
* Implementing Texture Shader and Rendering Pipeline
* Lighting shader and drawing pipeline implementation
* ~~Implementing camera movement and rotation~~(2025/04/28)
    * Figure out what to do about the one-frame delay in `RenderContext`.

## **3D Model**
* implementing Mixamo animation playback function
* Implementing a process to unify the `origin position` of 3D models

## **Game Objects(Actor)**
* Considering the architecture for easily creating Actor object classes
* Consider how to create an `Entity` for each `Mesh` instead of creating an `Entity` for each actor and manage them
    * Give `Entity` a hierarchical structure
    * Give `Entity` a name
    * Link `Entity` and `Actor` based on `name` (proposal)
* Implemented a simplified actor creation process by templating `Entity` and `Component`



## **System**
* Implementing a Texture Importer
* Implementing load log acquisition and display functions
* ~~WindowManager implementation~~(2025/04/29)
* Implementing the engine (development) camera and the game camera, and separating them
* **Implementation of meaningful data lists and search logic (naming entities and efficiently searching by name)**/**Separation from mechanical processing**

## **FUTURE**
* Implementing game editing functionality via UI (engine)
* Simplify prototyping with game templates
* **Scene-oriented prototyping support engine**