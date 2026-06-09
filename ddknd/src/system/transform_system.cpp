#include <ddknd/system/transform_system.h>

#include <ddknd/component/gfx_component.h>

namespace ddknd::system
{
    void TransformSystem::UpdateOne(component::TransformComponent& transform)
    {
        if(!transform.dirty)
            return;

        transform.worldMatrix = transform.localTRS.ToMatrix();
        transform.dirty = false;
    }
}