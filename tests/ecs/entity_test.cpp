#include <ddknd/ecs/ecs.h>

#include <iostream>

#define TEST_CHECK(condition)                                               \
    do                                                                      \
    {                                                                       \
        if(!(condition))                                                    \
        {                                                                   \
            std::cerr   << "FAILED: " #condition                            \
                        << " at " << __FILE__ << ":" << __LINE__            \
                        << '\n';                                            \
            ++failures;                                                     \
        }                                                                   \
    } while (false)                                                         

int main()
{
    int failures = 0;

    ddknd::ecs::World world;

    const auto entityA = world.Create();
    TEST_CHECK(world.IsAlive(entityA));

    world.Destroy(entityA);
    TEST_CHECK(!world.IsAlive(entityA));

    const auto entityB = world.Create();

    // Creating another entity must not revive the old handle.
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

    return failures;
}