#include <gtest/gtest.h>
#include "my_lib.hpp"
#include <exception>
#include <iostream>
using namespace std;

TEST(BigintTest, test_modn1) {
    BigInt x(8), y(2);
    ASSERT_TRUE(mod_n(x, y).is_null());
}

TEST(BigintTest, test_modn2) {
    BigInt x(8), z(3);
    ASSERT_EQ(mod_n(x, z), BigInt(2));
}

TEST(BigintTest, test_modn3) {
    BigInt x(8);
    ASSERT_EQ(mod_n(x, BigInt(1)), BigInt(0));
}

TEST(BigintTest, test_modn4) {
    BigInt x(8);
    ASSERT_THROW(mod_n(x, BigInt(0)), std::runtime_error);
}

TEST(BigintTest, test_modn5) {
    ASSERT_EQ(mod_n(BigInt(-7), BigInt(3)), BigInt(-1));
}

TEST(BigintTest, test_modexp1) {
    ASSERT_EQ(mod_exp(BigInt(123), BigInt(4), BigInt(7)), BigInt(4));
}

TEST(BigintTest, test_modexp2) {
    ASSERT_EQ(mod_exp(BigInt(12345), BigInt(6789), BigInt(167)), BigInt(14));
}

TEST(BigintTest, test_modexp3) {
    ASSERT_EQ(mod_exp(BigInt(3), BigInt(5), BigInt(30)), BigInt(3));
}

TEST(BigintTest, test_modexp4) {
    ASSERT_EQ(mod_exp(BigInt(249), BigInt(321), BigInt(499)), BigInt(447));
}

TEST(BigintTest, test_modexp5) {
    ASSERT_EQ(mod_exp(BigInt(5), BigInt(1), BigInt(8)), BigInt(5));
}

TEST(BigintTest, test_modexp6) {
    ASSERT_EQ(mod_exp(BigInt(5), BigInt(0), BigInt(8)), BigInt(1));
}

TEST(BigintTest, test_modexp7) {
    ASSERT_EQ(mod_exp(BigInt(3), BigInt("618970019642690137449562110"), BigInt("618970019642690137449562111")), BigInt(1));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}