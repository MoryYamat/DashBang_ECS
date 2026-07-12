#include <ddknd/ecs/ecs.h>
#include "support/test_check.h"


struct Pos
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    bool operator==(const Pos& o) const
    {
        return x == o.x && y == o.y && z == o.z;
    }
};

// Emplacing a component must associate it with the entity.
void TestEmplace()
{
    ddknd::ecs::World world;
    ddknd::ecs::Storage<Pos> storage;

    TEST_CHECK(storage.Size() == 0);

    const auto entity = world.Create();

    storage.Emplace(entity, Pos{1.0f, 1.0f, 1.0f});

    TEST_CHECK(storage.Size() == 1);
    TEST_CHECK(storage.Has(entity));

    const auto* stored = storage.Get(entity);
    TEST_CHECK(stored != nullptr);

    if (stored == nullptr)
    {
        return;
    }

    TEST_CHECK((*stored == Pos{1.0f, 1.0f, 1.0f}));
}


// Re-emplacing the same entity must replace the component
// without growing storage.
void TestReEmplaceReplacesComponent()
{
    ddknd::ecs::World world;
    ddknd::ecs::Storage<Pos> storage;

    const auto entity = world.Create();

    storage.Emplace(entity, Pos{1.0f, 1.0f, 1.0f});
    storage.Emplace(entity, Pos{2.0f, 2.0f, 2.0f});

    TEST_CHECK(storage.Size() == 1);

    const auto* stored = storage.Get(entity);
    TEST_CHECK(stored != nullptr);

    if (stored == nullptr)
    {
        return;
    }

    TEST_CHECK((*stored == Pos{2.0f, 2.0f, 2.0f}));
}


// Removing an entity without a component must not change the storage.
void TestRemove()
{
    ddknd::ecs::World world;
    ddknd::ecs::Storage<Pos> storage;

    const auto entityA = world.Create();
    const auto entityB = world.Create();

    storage.Emplace(entityA, Pos{1.0f, 1.0f, 1.0f});

    // Removing an entity without a component must not change the storage.
    storage.Remove(entityB);
    TEST_CHECK(storage.Size() == 1);

    // Removing a stored component must make it inaccessible.
    storage.Remove(entityA);

    TEST_CHECK(storage.Size() == 0);
    TEST_CHECK(!storage.Has(entityA));
    TEST_CHECK(storage.Get(entityA) == nullptr);
}


// Removing middle element must preserve the moved last element
// and update its sparse index.
void TestSwapAndPopKeepsStorageConsistent()
{
    ddknd::ecs::World world;
    ddknd::ecs::Storage<Pos> storage;

    const auto entityA = world.Create();
    const auto entityB = world.Create();
    const auto entityC = world.Create();

    storage.Emplace(entityA, Pos{1.0f, 1.0f, 1.0f});
    storage.Emplace(entityB, Pos{2.0f, 2.0f, 2.0f});
    storage.Emplace(entityC, Pos{3.0f, 3.0f, 3.0f});

    storage.Remove(entityB);

    TEST_CHECK(storage.Size() == 2);
    TEST_CHECK(storage.Has(entityA));
    TEST_CHECK(!storage.Has(entityB));
    TEST_CHECK(storage.Has(entityC));

    const auto* remainingA = storage.Get(entityA);
    const auto* movedC = storage.Get(entityC);

    TEST_CHECK(remainingA != nullptr);
    TEST_CHECK(movedC != nullptr);

    if (remainingA == nullptr || movedC == nullptr)
    {
        return;
    }

    TEST_CHECK((*remainingA == Pos{1.0f, 1.0f, 1.0f}));
    TEST_CHECK((*movedC == Pos{3.0f, 3.0f, 3.0f}));

    TEST_CHECK(storage.EntityAt(0) == entityA);
    TEST_CHECK(storage.EntityAt(1) == entityC);
    TEST_CHECK((storage.ComponentAt(0) == Pos{1.0f, 1.0f, 1.0f}));
    TEST_CHECK((storage.ComponentAt(1) == Pos{3.0f, 3.0f, 3.0f}));
}

int main()
{
    TestEmplace();
    TestReEmplaceReplacesComponent();
    TestRemove();
    TestSwapAndPopKeepsStorageConsistent();

    return ddknd::tests::failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}