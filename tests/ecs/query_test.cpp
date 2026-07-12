#include <ddknd/ecs/ecs.h>

#include <type_traits>

struct Pos{};
struct Vel{};
struct Acc{};

int main()
{
    // select specifies the component used for iteration
    // require specifies additional required components, 
    // exclude specifies components that must not be present.
    auto query = ddknd::ecs::query().select<Pos>()
                                    .require<Vel>()
                                    .exclude<Acc>();

    static_assert(std::is_same_v<decltype(query) ,ddknd::ecs::Query<Pos, ddknd::ecs::TypeList<Vel>, ddknd::ecs::TypeList<Acc>>>);
    
    return EXIT_SUCCESS;
}