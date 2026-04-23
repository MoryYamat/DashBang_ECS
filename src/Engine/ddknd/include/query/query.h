#pragma once

#include <vector>

#include "registry/registry.h"
#include "entity/entity.h"

namespace ddknd::query
{ 
    // Query specific entities that satisfy the specified constraints.

    // Primitives
    template<typename... Args>
    struct All
    {
        using Registry = ::ddknd::registry::Registry;
        using Entity = ::ddknd::entity::Entity;

        bool operator()(const Registry& reg, Entity e) const
        {
            return ((reg.TryGetComponent<Args>(e)!=nullptr) && ...);
        }
    };

    template<typename...Args>
    struct Any
    {
        using Registry = ::ddknd::registry::Registry;
        using Entity = ::ddknd::entity::Entity;

        bool operator()(const Registry& reg, Entity e) const
        {
            if constexpr (sizeof...(Args) == 0) return true;
            return ((reg.TryGetComponent<Args>(e) != nullptr ) || ...);
        }
    };

    // ======= Composer ======= 
    template<typename L, typename R>
    struct And
    {
        using Registry = ::ddknd::registry::Registry;
        using Entity = ::ddknd::entity::Entity;

        L l;
        R r;
        bool operator()(const Registry& reg,  Entity e) const {return l(reg, e) && r(reg, e);}
    };

    
    template<typename L, typename R>
    struct Or
    {
        using Registry = ::ddknd::registry::Registry;
        using Entity = ::ddknd::entity::Entity;

        L l;
        R r;
        bool operator()(const Registry& reg,  Entity e) const {return l(reg, e) || r(reg, e);}
    };

    
    template<typename P>
    struct Not
    {
        P p;
        
        using Registry = ::ddknd::registry::Registry;
        using Entity = ::ddknd::entity::Entity;
        bool operator()(const Registry& reg,  Entity e) const {return !p(reg,e);}
    };
}// namespace ddknd::query