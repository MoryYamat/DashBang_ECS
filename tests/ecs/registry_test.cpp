#include <ddknd/ecs/ecs.h>

#include <iostream>


int failures = 0;

void test_check_impl(bool condition, const char* expression, const char* file, int line)
{
    if(!condition)
    {
            std::cerr   << "FAILED: " << expression
                        << " at "     << file << ":" << line
                        << '\n';

            ++failures;
    }
}

#define TEST_CHECK(condition)   \
    test_check_impl((condition), #condition, __FILE__, __LINE__)

struct Pos
{
    float x;
    float y;
    float z;
};

int main()
{
    ddknd::ecs::World world;

    const auto entity = world.Create();

    auto& reg = world.GetRegistry();
    reg.AddComponent<Pos>(entity, Pos{1.0f, 1.0f, 1.0f});

    auto* e_pos = reg.TryGetComponent<Pos>(entity);
    TEST_CHECK(e_pos);

    return failures;
}