#include <ddknd/ecs/ecs.h>

#include <iostream>

#include "support/test_check.h"

struct Pos
{
    float x;
    float y;
    float z;
};

int main()
{
    ddknd::ecs::World world;

    const auto entityA = world.Create();
    const auto entityB = world.Create();

    auto& reg = world.GetRegistry();
    
    // An added component must be associated only with entity.
    reg.AddComponent<Pos>(entityA, Pos{1.0f, 1.0f, 1.0f});

    TEST_CHECK(reg.HasComponent<Pos>(entityA));
    TEST_CHECK(!reg.HasComponent<Pos>(entityB));

    auto* posA = reg.TryGetComponent<Pos>(entityA);
    auto* posB = reg.TryGetComponent<Pos>(entityB);
    TEST_CHECK(posA);
    TEST_CHECK(!posB);

    // GetComponent must return the stored component
    auto& storedPosA = reg.GetComponent<Pos>(entityA);
    TEST_CHECK(storedPosA.x == 1.0f && storedPosA.y == 1.0f && storedPosA.z == 1.0f);

    // Changes through the returned reference must affect sotred data.
    storedPosA.x = 2.0f;
    storedPosA.y = 2.0f;
    storedPosA.z = 2.0f;
    TEST_CHECK(storedPosA.x == 2.0f && storedPosA.y == 2.0f && storedPosA.z == 2.0f);

    // A removed component must no longer be associated with the entity.
    reg.RemoveComponent<Pos>(entityA);
    TEST_CHECK(!reg.HasComponent<Pos>(entityA));
    TEST_CHECK(reg.TryGetComponent<Pos>(entityA) == nullptr);
    
    return ddknd::tests::failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}