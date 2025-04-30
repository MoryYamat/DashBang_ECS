# **Identifying issues in the development process and consolidating solutions**

### **Project**
* Convert the local character code from `Shift-JIS` to `UTF-8` in bulk
* **Organizing the folder structure**

## **Architecture
* Separation of game logic and system (engine) logic

## **ECS**
* Defining hierarchical relationships between `Entity` and implementing semantics based on names
* *** Consider a good way to manage the lifespan of `Entity` (even if you delete an `Entity`, the `Component` will remain. If there is a process that references the `Entity`, you need to manage the deletion carefully)**

## **RendererSystem**
* ~~Accurately extracting OpenGL buffers such as VAO and VBO from `MeshComponent` and coordinating `ModelData`~~(2025/04/26)
    * The solution is to add a for loop and handle intermediate data.
* Implementing Texture Shader and Rendering Pipeline
* Lighting shader and drawing pipeline implementation
* Implementing camera movement and rotation

## **3D Animation**
* implementing Mixamo animation playback function

## **Game Objects(Actor)**
* Considering the architecture for easily creating Actor object classes
* Consider how to create an `Entity` for each `Mesh` instead of creating an `Entity` for each actor and manage them
    * Give `Entity` a hierarchical structure
    * Give `Entity` a name
    * Link `Entity` and `Actor` based on `name` (proposal)

## **Physical Layer**
* Physics layer (collision) implementation
* Ground implementation

## **System**
* Implementing a Texture Importer
* Implementing load log acquisition and display functions
* WindowManager implementation


## **FUTURE**
* Implementing game editing functionality via UI (engine)
* Simplify prototyping with game templates
* **Scene-oriented prototyping support engine**