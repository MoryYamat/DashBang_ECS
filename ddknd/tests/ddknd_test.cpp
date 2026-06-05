#include <gtest/gtest.h>



// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

TEST(AssetMGRTest, BasicAssertions)
{
  // ::ddknd::asset::AssetManager mgr();
  // const auto id = mgr.GetOrCreate<ddknd::asset::type::ShaderResource>("hello");
  // EXPECT_EQ(id.Id().Value(), 0);
}
