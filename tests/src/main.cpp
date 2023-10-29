#include <gtest/gtest.h>

auto main(int argc, char** argv) -> int
{
    using namespace testing;
    InitGoogleTest(&argc, argv);
    return UnitTest::GetInstance()->Run();
}
