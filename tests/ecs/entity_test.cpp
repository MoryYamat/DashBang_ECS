#include <ddknd/ecs/ecs.h>

#include "support/test_check.h"

int main()
{
    int failures = 0;

    ddknd::ecs::World world;

    // A newly created entity is alive.
    const auto entityA = world.Create();
    TEST_CHECK(world.IsAlive(entityA));

    // Adestroyed entity is no longer alive.
    world.Destroy(entityA);
    TEST_CHECK(!world.IsAlive(entityA));

    // Reusing an index must not revive the old handle.
    const auto entityB = world.Create();

    TEST_CHECK(world.IsAlive(entityB));
    TEST_CHECK(!world.IsAlive(entityA));
    TEST_CHECK(entityA.Index() == entityB.Index());
    TEST_CHECK(entityA.Gen() != entityB.Gen());

    // Destroying one entity must not affect another.
    const auto entityC = world.Create();
    TEST_CHECK(world.IsAlive(entityC));

    world.Destroy(entityB);

    TEST_CHECK(!world.IsAlive(entityB));
    TEST_CHECK(world.IsAlive(entityC));

    // A default-constructed entity is invalid.
    ddknd::ecs::Entity entityD{};

    TEST_CHECK(!entityD.IsValid());

    entityD = world.Create();

    TEST_CHECK(entityD.IsValid());
    TEST_CHECK(world.IsAlive(entityD));

    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}