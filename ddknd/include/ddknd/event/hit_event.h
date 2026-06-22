/*
Temporary design until Event Manager etc. are created.
*/

#include <ddknd/ecs/entity/entity.h>
#include <vector>


namespace ddknd::event
{
    struct HitboxHitEvent
    {
        ddknd::ecs::Entity hitbox;
        ddknd::ecs::Entity owner;
        ddknd::ecs::Entity target;
    };
    struct HitboxHitEventBuffer
    {
        std::vector<HitboxHitEvent> events;

        void Clear()
        {
            events.clear();
        }

        void Push(HitboxHitEvent event)
        {
            events.push_back(event);
        }
    };
}