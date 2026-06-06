#pragma once

#include <memory>

#include "ddknd/ecs/entity/entity_fwd.h"
#include "ddknd/ecs/registry/registry_fwd.h"

namespace ddknd::ecs
{
    // orchestrator
    class World
    {
        public:
            using Entity = ::ddknd::ecs::Entity;
            using Registry = ::ddknd::ecs::Registry;
            World();
            ~World();
            
            // Entity CRUD
            Entity Create();
            void Destroy(Entity e);
            bool IsAlive(Entity e) const;
            // ==== ComponentCRUD ==== 
            // AddComponent
            // GetComponent
            // RemoveComponent
            // ==== Query/View ==== 
            // 条件に合うEntity/Component群を走査する
            Registry& GetRegistry();
            const Registry& GetRegistry() const;

        private:
            struct Impl;
            std::unique_ptr<Impl> impl_;
    };
}// namespace ddknd::world