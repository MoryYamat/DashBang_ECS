#pragma once

#include <memory>

#include <ddknd/ecs/entity/entity_fwd.h>
#include <ddknd/ecs/registry/registry_fwd.h>

namespace ddknd::ecs
{
    /**
    * @brief Owns the ECS registry and exposes the world-level entity API.
    */
    class World
    {
        public:
            using Entity = ::ddknd::ecs::Entity;
            using Registry = ::ddknd::ecs::Registry;
            World();
            ~World();
            
            // Entity lifetime
            Entity Create();
            void Destroy(Entity e);
            bool IsAlive(Entity e) const;
            
            /**
            * @brief Returns the underlying registry for component and query operations.
            */
            Registry& GetRegistry();
            const Registry& GetRegistry() const;

        private:
            struct Impl;
            std::unique_ptr<Impl> impl_;
    };
}// namespace ddknd::world