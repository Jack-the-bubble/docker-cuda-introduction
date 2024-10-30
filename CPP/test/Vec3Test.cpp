#include "Vec3.h"

#include <gtest/gtest.h>


TEST(Vec3Test, AccessingValuesTest) {
    const auto v1 = RT::Vec3(1, 2, 3);
    EXPECT_EQ(v1[0], 1);
    EXPECT_EQ(v1[1], 2);
    EXPECT_EQ(v1[2], 3);
    EXPECT_EQ(v1.x(), 1);
    EXPECT_EQ(v1.y(), 2);
    EXPECT_EQ(v1.z(), 3);
    EXPECT_EQ(v1.r(), 1);
    EXPECT_EQ(v1.g(), 2);
    EXPECT_EQ(v1.b(), 3);
}