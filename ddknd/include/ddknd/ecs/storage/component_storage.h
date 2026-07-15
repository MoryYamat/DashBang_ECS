#pragma once

#include <cstddef>
#include <limits>
#include <span>
#include <utility>
#include <vector>

#include <ddknd/ecs/entity/entity.h>

namespace ddknd::ecs
{
    struct IStorage
    {
        using Entity = ::ddknd::ecs::Entity;
        virtual ~IStorage() = default;
        virtual bool Has(Entity e) const = 0;
        virtual void Remove(Entity e) = 0;
    };

    // Sparse-set storage for a single component type.
    // References and pointers to components may be invalidated by Emplace or Remove.
    template <typename T>
    class Storage final : public IStorage
    {
      public:
        using Entity = ::ddknd::ecs::Entity;
        using comp_type = T;

        template <typename... Args>
        T& Emplace(Entity e, Args&&... args)
        {
            EnsureSparseSize(e);

            if (Has(e))
            {
                const std::size_t sparseIdx = static_cast<std::size_t>(e.Index());
                const std::size_t denseIdx = sparse_[sparseIdx];
                components_[denseIdx] = T{std::forward<Args>(args)...};
                return components_[denseIdx];
            }

            const std::size_t denseIdx = components_.size();
            sparse_[static_cast<std::size_t>(e.Index())] = denseIdx;
            ents_.push_back(e);
            components_.emplace_back(std::forward<Args>(args)...);
            return components_.back();
        }

        // Removes the component with swap-and-pop.
        // The dense storage order is not preserved.
        void Remove(Entity e) override
        {
            if (!Has(e))
            {
                return;
            }

            const std::size_t sparseIdx = static_cast<std::size_t>(e.Index());
            const std::size_t denseIdx = sparse_[sparseIdx];
            const std::size_t lastIdx = components_.size() - 1;
            const Entity lastEntity = ents_[lastIdx];

            if (denseIdx != lastIdx)
            {
                components_[denseIdx] = std::move(components_[lastIdx]);
                ents_[denseIdx] = lastEntity;
                sparse_[static_cast<std::size_t>(lastEntity.Index())] = denseIdx;
            }

            components_.pop_back();
            ents_.pop_back();
            sparse_[sparseIdx] = npos;
        }

        T* Get(Entity e)
        {
            if (!Has(e))
            {
                return nullptr;
            }

            return &components_[sparse_[static_cast<std::size_t>(e.Index())]];
        }

        const T* Get(Entity e) const
        {
            if (!Has(e))
            {
                return nullptr;
            }

            return &components_[sparse_[static_cast<std::size_t>(e.Index())]];
        }

        std::span<const T> Components() const
        {
            return std::span<const T>{components_};
        }

        std::span<T> Components()
        {
            return std::span<T>{components_};
        }

        std::span<const Entity> Entities() const
        {
            return std::span<const Entity>{ents_};
        }

        bool Has(Entity e) const override
        {
            if (!e.IsValid())
            {
                return false;
            }
            const std::size_t sparseIdx = e.Index();
            if (sparseIdx >= sparse_.size())
            {
                return false;
            }
            const std::size_t denseIdx = sparse_[sparseIdx];
            if (denseIdx == npos)
            {
                return false;
            }
            return denseIdx < ents_.size() && ents_[denseIdx] == e;
        }

        std::size_t Size() const
        {
            return components_.size();
        }

        Entity EntityAt(std::size_t i) const
        {
            return ents_[i];
        }

        T& ComponentAt(std::size_t i)
        {
            return components_[i];
        }

        const T& ComponentAt(std::size_t i) const
        {
            return components_[i];
        }

      private:
        void EnsureSparseSize(Entity entity)
        {
            const std::size_t idx = static_cast<std::size_t>(entity.Index());
            if (idx >= sparse_.size())
            {
                sparse_.resize(idx + 1, npos);
            }
        }

      private:
        static constexpr std::size_t npos = std::numeric_limits<std::size_t>::max();
        std::vector<T> components_;
        std::vector<Entity> ents_;
        std::vector<std::size_t> sparse_;
    };
} // namespace ddknd::ecs