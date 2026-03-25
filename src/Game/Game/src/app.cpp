#include "app.h"


namespace app::test
{
    void test()
    {
        ddknd::asset::AssetManager mgr;
        const auto id = mgr.GetOrCreate<ddknd::asset::type::ShaderResource>("hello");
        std::cout << "id: " << static_cast<int>(id.Id().Value());
    }
}// namespace app::test