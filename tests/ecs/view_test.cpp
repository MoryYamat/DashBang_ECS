#include <ddknd/ecs/ecs.h>

#include <cstdint>

#include "support/test_check.h"

struct Pos
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    bool operator==(const int o) const
    {
        return x == o && y == o && z == o;
    }

    bool operator==(const Pos& o) const
    {
        return x == o.x && y == o.y && z == o.z;
    }

    Pos& operator+=(const Pos& o)
    {
        this->x += o.x;
        this->y += o.y;
        this->z += o.z;
        return *this;
    }

    Pos& operator+=(const int o)
    {
        this->x += o;
        this->y += o;
        this->z += o;
        return *this;
    }
};

struct Vel
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    bool operator==(const int o) const
    {
        return x == o && y == o && z == o;
    }

    bool operator==(const Vel& o) const
    {
        return x == o.x && y == o.y && z == o.z;
    }

    Vel& operator+=(const Vel& o)
    {
        this->x += o.x;
        this->y += o.y;
        this->z += o.z;
        return *this;
    }

    Vel& operator+=(const int o)
    {
        this->x += o;
        this->y += o;
        this->z += o;
        return *this;
    }
};

struct Acc
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

int main()
{
    ddknd::ecs::World world;

    auto& reg = world.GetRegistry();

    const auto entityA = reg.Create();
    const auto entityB = reg.Create();
    const auto entityC = reg.Create();

    reg.AddComponent<Pos>(entityA);
    reg.AddComponent<Vel>(entityA);

    reg.AddComponent<Vel>(entityB);
    reg.AddComponent<Pos>(entityB);

    reg.AddComponent<Vel>(entityC);
    reg.AddComponent<Pos>(entityC);
    reg.AddComponent<Acc>(entityC);

    // The view must iterate only over entities satisfying
    // select, require, and exclude.
    auto query = ddknd::ecs::query().select<Pos>()
                                    .require<Vel>()
                                    .exclude<Acc>();

    std::size_t entityCount = 0;

    for(auto [e, pos, vel] : reg.view(query).withEntity())
    {
        entityCount++;

        TEST_CHECK(entityC != e);
        TEST_CHECK(entityA == e || entityB == e);
        TEST_CHECK(reg.HasComponent<Pos>(e));
        TEST_CHECK(reg.HasComponent<Vel>(e));
        TEST_CHECK(!reg.HasComponent<Acc>(e));
    }

    TEST_CHECK(entityCount == 2);

    // Strucutred bindings must provide access to stored components.
    std::size_t componentCount = 0;

    for(auto [pos, vel] : reg.view(query))
    {
        componentCount++;

        pos += 1;
        vel += 1;
        TEST_CHECK(pos == 1);
        TEST_CHECK(vel == 1);
    }

    TEST_CHECK(componentCount == 2);

    // Changes made through the view must remain stored in the Registry.
    TEST_CHECK(reg.GetComponent<Pos>(entityA) == 1);
    TEST_CHECK(reg.GetComponent<Vel>(entityA) == 1);
    TEST_CHECK(reg.GetComponent<Pos>(entityB) == 1);
    TEST_CHECK(reg.GetComponent<Vel>(entityB) == 1);

    return ddknd::tests::failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}