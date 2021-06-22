/**
 * @file    main.cpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    21.05.2021
 */

EXT_DISABLE_WARNINGS_PUSH
#   include <gtest/gtest.h>
EXT_DISABLE_WARNINGS_POP

auto main(int argc, char** argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
