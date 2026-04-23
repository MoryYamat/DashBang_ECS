// Manage one type of component
// sparse-set ecs
#pragma once

#include <cstddef>
#include <limits>
#include <vector>

#include "entity/entity.h"

namespace ddknd::storage
{
    inline constexpr std::size_t npos = std::numeric_limits<std::size_t>::max();
    struct IStorage
    {
        using Entity = ::ddknd::entity::Entity;
        virtual ~IStorage() = default;
        virtual bool Has(Entity e) const = 0;
        virtual void Remove(Entity e) = 0;
    };

    // Component CRUD
    template<typename T>
    class Storage final : public IStorage
    {
        public:
        using Entity = ::ddknd::entity::Entity;
        using comp_type = T;

        template<typename ...Args>
        T& Emplace(Entity e, Args&& ...args)
        {
            EnsureSparseSize(e);

            if(Has(e))
            {
                const std::size_t sparseIdx = static_cast<std::size_t>(e.Index());
                auto idx = sparse_[sparseIdx];
                components_[idx] = T{std::forward<Args>(args)...};
                return components_[idx];
            }

            const std::size_t idx = components_.size();
            sparse_[e.Index()] = idx;
            ents_.push_back(e);
            components_.emplace_back(std::forward<Args>(args)...);
            return components_.back();
        }

        void Remove(Entity e) override
        {
            if(!Has(e))
            {
                return;
            }

            const std::size_t sparseIdx = e.Index();
            const std::size_t idx = sparse_[sparseIdx];
            const std::size_t last = components_.size() - 1;
            const Entity lastEntity = ents_[last];

            if(idx != last)
            {
                components_[idx] = std::move(components_[last]);
                ents_[idx] = lastEntity;
                sparse_[lastEntity.Index()] = idx;
            }

            components_.pop_back();
            ents_.pop_back();
            sparse_[sparseIdx] = npos;
        }

        T* Get(Entity e)
        {
            if(!Has(e))
            {
                return nullptr;
            }

            return &components_[sparse_[static_cast<std::size_t>(e.Index())]];
        }
        
        const T* Get(Entity e) const
        {
            if(!Has(e))
            {
                return nullptr;
            }

            return &components_[sparse_[static_cast<std::size_t>(e.Index())]];
        }

        const std::vector<T>& Components() const
        {
            return components_;
        }

        std::vector<T>& Components()
        {
            return components_;
        }

        bool Has(Entity e) const override
        {
            if(!e.IsValid())
            {
                return false;
            }
            const std::size_t sparseIdx = e.Index();
            if(sparseIdx >= sparse_.size())
            {
                return false;
            }
            const std::size_t denseIdx = sparse_[sparseIdx];
            if(denseIdx == npos)
            {
                return false;
            }
            return denseIdx < ents_.size() && ents_[denseIdx] == e;
        }

        private:
        void EnsureSparseSize(Entity entity)
        {
            const std::size_t idx = static_cast<std::size_t>(entity.Index());
            if(idx >= sparse_.size())
            {
                sparse_.resize(idx + 1, npos);
            }
        }

        private:
            std::vector<T> components_;
            std::vector<Entity> ents_;
            std::vector<std::size_t> sparse_;
    };
}// namespace ddknd::storage

// AoS
// struct Position
// {
//     float x,y,z;
// }
// struct AoS
// {
//     std::vector<Position> s;
// };

// SoA
// struct Position
// {
//     std::vector<float> x;
//     std::vector<float> y;
//     std::vector<float> z;
// };


// AoSoA
// struct Position3xWord
// {
//     std::size_t word = 8;
//     std::vector<float> x;
//     std::vector<float> y;
//     std::vector<float> z;
// };
// struct Position
// {
//     std::vector<Position3xWord> pos;
//     float GetPos(Index i){ return pos[i / word].x[i % word];}
// };
