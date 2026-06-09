#pragma once

namespace ddknd::component
{
    struct TransformComponent;
}


namespace ddknd::system
{
    class TransformSystem
    {
        public:
            static void UpdateOne(component::TransformComponent& transform);
    };
}