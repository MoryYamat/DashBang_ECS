#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>

#include "entity/entity.h"
#include "storage/component_storage.h"

namespace ddknd::world
{
    // orchestrator
    class World
    {
        public:
            // Entity CRUD
            using Entity = ::ddknd::entity::Entity;

            // ==== ComponentCRUD ==== 
            // AddComponent
            // GetComponent
            // RemoveComponent
            // ==== Query/View ==== 
            // 条件に合うEntity/Component群を走査する
            // template<typename T>
            // storage::Storage<T>& GetStorage()
            // {
            //     const std::type_index key{typeid(T)};

            //     auto it = storages_.find(key);
            //     if(it == storages_.end())
            //     {
            //         auto ptr = std::make_unique<storage::Storage<T>>();
            //         auto* raw = ptr.get();
            //         storages_.emplace(key, std::move(ptr));
            //         return *raw;
            //     }

            //     return *static_cast<storage::Storage<T>*>(it->second.get());
            // }
        private:
    };
}// namespace ddknd::world