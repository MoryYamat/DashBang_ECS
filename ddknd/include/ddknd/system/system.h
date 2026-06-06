#pragma once

namespace ddknd::world
{
    class World;
}

namespace ddknd::system
{
    struct SystemContext
    {

    };

    // Scheduler
    // topological sort 
    // parallelism
    // 
    class EngineSystemRunner
    {
        public:
            void Update(::ddknd::world::World& world, const ::ddknd::system::SystemContext& ctx);
        private:
            void RunTransform(::ddknd::world::World& world, const ::ddknd::system::SystemContext& ctx);
    };
}// namespace ddknd::system

// API Image
// auto view = registry.view(.query().select<AComp>().require<BComp>());
// view.each([](Acomp& a, Bcomp& b){ABSystem::Updateone(a,b);});
