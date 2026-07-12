#include <ddknd/ecs/ecs.h>

#include "support/test_check.h"

struct Pos
{
    float x{};
};

void TestWorldExposesItsRegistry()
{
    ddknd::ecs::World world;

    const auto entity = world.Create();
    auto& registry = world.GetRegistry();

    TEST_CHECK(registry.IsAlive(entity));
}

void TestWorldAndRegistryShareTheSameState()
{
    ddknd::ecs::World world;

    const auto entity = world.Create();
    auto& registry = world.GetRegistry();

    registry.AddComponent<Pos>(entity, Pos{1.0f});

    TEST_CHECK(registry.HasComponent<Pos>(entity));

    world.Destroy(entity);

    TEST_CHECK(!world.IsAlive(entity));
    TEST_CHECK(!registry.HasComponent<Pos>(entity));
}

int main()
{
    TestWorldExposesItsRegistry();
    TestWorldAndRegistryShareTheSameState();

    return ddknd::tests::failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}