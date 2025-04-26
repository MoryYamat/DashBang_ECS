// mesh data component

#include <GLAD/glad.h>

#include <GLM/glm.hpp>

#include "DataTypes/ModelData.h"

struct MeshComponent
{
	// Mesh Resources
	ModelData modelData;
	ModelGPU modelGPU;

};

// memo 
// SIMD single instruction multiple data (CPU and memory/cache)
//