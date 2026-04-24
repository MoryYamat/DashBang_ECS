#pragma once

#include <bitset>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>
#include <typeindex>

#include "entity/entity.h"
#include "storage/component_storage.h"
// #include "query/query_fwd.h"// fwd

#include <spdlog/spdlog.h>

namespace ddknd::registry
{
    // Storage set + α
    // Entity CRUD
    // Storage Management
    // Provides API
    class Registry
    {
        using id_type = ::ddknd::entity::Entity;

      private:
        std::vector<std::uint32_t> gens_; // current generation: gens_[i] = gen (i=Index, gen=currentGeneration)
        std::vector<std::uint32_t> free_indices_;
        std::unordered_map<std::type_index, std::unique_ptr<::ddknd::storage::IStorage>> storages_;
      public:
        // Entity
        id_type Create()
        {
            std::uint32_t idx;
            // recycle
            if (!free_indices_.empty())
            {
                idx = free_indices_.back();
                free_indices_.pop_back();
            }
            else // new
            {
                idx = static_cast<std::uint32_t>(gens_.size());
                gens_.push_back(0);
            }

            std::cerr << "new Entity: gen=" << std::bitset<32>(gens_[idx]) << " idx=" << std::bitset<32>(idx) << "\n";
            return id_type{gens_[idx], idx};
        }

        void Destroy(id_type e)
        {
            if (!IsAlive(e))
            {
                return;
            }

            for(auto& [key, storage]: storages_)
            {
                storage->Remove(e);
            }

            assert(e.Index() < gens_.size());

            const auto idx = e.Index();
            ++gens_[idx];
            free_indices_.push_back(idx);
        }

        bool IsAlive(id_type e) const
        {
            const auto idx = e.Index();
            return idx < gens_.size() && gens_[idx] == e.Gen();
        }

        // if storage<T> already exists then register that entity
        // if doesn't exist make storage<T> and register entity
        template <typename T, typename... Args>
        T& AddComponent(id_type e, Args&&... a)
        {
            assert(IsAlive(e));
            return AssureStorage<T>().Emplace(e, std::forward<Args>(a)...);
        }

        template<typename T>
        bool HasComponent(id_type e) const
        {
            auto it = storages_.find(typeid(T));
            return it != storages_.end() && it->second->Has(e);
        }

        template<typename T>
        T* TryGetComponent(id_type e)
        {
            auto it = storages_.find(typeid(T));
            if(it == storages_.end()) return nullptr;
            auto* raw = static_cast<storage::Storage<T>*>(it->second.get());
            return raw->Get(e);
        }

        template<typename T>
        const T* TryGetComponent(id_type e) const
        {
            auto it = storages_.find(typeid(T));
            if(it == storages_.end()) return nullptr;
            auto* raw = static_cast<storage::Storage<T>*>(it->second.get());
            return raw->Get(e);
        }

        template<typename T>
        T& GetComponent(id_type e)
        {
            auto* p = TryGetComponent<T>(e);
            if(!p)
            {
                assert(false && "Get<T>: component missing");
            }
            return *p;
        }

        template<typename T>
        const T& GetComponent(id_type e) const
        {
            auto* p = TryGetComponent<T>(e);
            if(!p)
            {
                assert(false && "Get<T>: component missing");
            }
            return *p;
        }

        template<typename T>
        void RemoveComponent(id_type e)
        {
            auto it = storages_.find(typeid(T));
            if(it == storages_.end()) return;
            auto* ptr = it->second.get();
            ptr->Remove(e);
            return;
        }

        template<typename T>
        storage::Storage<T>& AssureStorage()
        {
            const std::type_index key{typeid(T)};
            auto it = storages_.find(key);
            if(it == storages_.end())
            {
                auto ptr = std::make_unique<storage::Storage<T>>();
                auto* raw = ptr.get();
                storages_.emplace(key, std::move(ptr));
                return *raw;
            }
            return *static_cast<storage::Storage<T>*>(it->second.get());
        }

        template<typename Query>
        auto view(Query q);
    };
} // namespace ddknd::registry

// Entity Management の要点
// - EntityID = Generation + Index
// - CreateEntity: 無効Indexの空きを埋める-> 最小の無効Indexを検出
//      - 無効Index-> currentGen != prevGen or nextGen == currentGen
//      - 最小Index( )