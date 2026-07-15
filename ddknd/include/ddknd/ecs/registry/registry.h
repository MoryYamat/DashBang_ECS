#pragma once

#include <cassert>
#include <cstdint>
#include <exception>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>


#include <ddknd/ecs/entity/entity.h>
#include <ddknd/ecs/storage/component_storage.h>


namespace ddknd::ecs
{
    class Registry
    {
        using id_type = ::ddknd::ecs::Entity;

      private:
        std::vector<std::uint32_t> gens_;   // Current generation for each entity index
        std::vector<std::uint32_t> free_indices_;
        std::unordered_map<std::type_index, std::unique_ptr<::ddknd::ecs::IStorage>> storages_;

      public:
        id_type Create()
        {
            std::uint32_t idx;

            if (!free_indices_.empty())
            {
                idx = free_indices_.back();
                free_indices_.pop_back();
            }
            else
            {
                idx = static_cast<std::uint32_t>(gens_.size());
                gens_.push_back(0);
            }

            return id_type{gens_[idx], idx};
        }

        void Destroy(id_type e)
        {
            if (!IsAlive(e))
            {
                return;
            }

            for (auto& entry : storages_)
            {
                entry.second->Remove(e);
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

        template <typename T, typename... Args>
        T& AddComponent(id_type e, Args&&... a)
        {
            assert(IsAlive(e));
            return AssureStorage<T>().Emplace(e, std::forward<Args>(a)...);
        }

        template <typename T>
        bool HasComponent(id_type e) const
        {
            auto it = storages_.find(typeid(T));
            return it != storages_.end() && it->second->Has(e);
        }

        template <typename T>
        T* TryGetComponent(id_type e)
        {
            auto it = storages_.find(typeid(T));
            if (it == storages_.end())
            {
                return nullptr;
            }

            auto* raw = static_cast<ecs::Storage<T>*>(it->second.get());
            return raw->Get(e);
        }

        template <typename T>
        const T* TryGetComponent(id_type e) const
        {
            auto it = storages_.find(typeid(T));
            if (it == storages_.end())
            {
                return nullptr;
            }

            auto* raw = static_cast<ecs::Storage<T>*>(it->second.get());
            return raw->Get(e);
        }

        template <typename T>
        T& GetComponent(id_type e)
        {
            auto* p = TryGetComponent<T>(e);
            if (!p)
            {
                assert(false && "GetComponent<T>: component missing");
                std::terminate();
            }
            return *p;
        }

        template <typename T>
        const T& GetComponent(id_type e) const
        {
            auto* p = TryGetComponent<T>(e);
            if (!p)
            {
                assert(false && "GetComponent<T>: component missing");
                std::terminate();
            }
            return *p;
        }

        template <typename T>
        void RemoveComponent(id_type e)
        {
            auto it = storages_.find(typeid(T));
            if (it == storages_.end())
            {
                return;
            }

            it->second->Remove(e);
        }

        template <typename T>
        ecs::Storage<T>& AssureStorage()
        {
            const std::type_index key{typeid(T)};
            auto it = storages_.find(key);
            if (it == storages_.end())
            {
                auto ptr = std::make_unique<ecs::Storage<T>>();
                auto* raw = ptr.get();
                storages_.emplace(key, std::move(ptr));
                return *raw;
            }
            return *static_cast<ecs::Storage<T>*>(it->second.get());
        }

        template <typename Query>
        auto view(Query q);
    };
} // namespace ddknd::ecs
