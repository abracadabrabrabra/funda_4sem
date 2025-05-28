#include <gtest/gtest.h>
#include "my_lib.hpp"
#include <exception>
#include <iostream>
#include <sstream>
using namespace std;

TEST(BigintTest, compare_test) {
    BigInt a(10), b(10), c(9), d(-3);
    ASSERT_EQ(a == b, true);
    ASSERT_EQ(a != b, false);
    ASSERT_EQ(a == c, false);
    ASSERT_EQ(a != c, true);
    ASSERT_TRUE(c < a);
    ASSERT_TRUE(b <= a);
    ASSERT_TRUE(a > c);
    ASSERT_TRUE(a >= b);
    ASSERT_TRUE(a > d);
    ASSERT_TRUE(d < c);
}

TEST(BigintTest, test_constr) {
    vector<unsigned short> a{ 0, 1, 2 };
    BigInt x(210);
    ASSERT_EQ(a, x.digits());
    ASSERT_EQ(1, x.sign_());
    BigInt y("-000210");
    ASSERT_EQ(a, y.digits());
    ASSERT_EQ(-1, y.sign_());
    BigInt z;
    ASSERT_TRUE(z.digits().empty());
    BigInt w(x), w1 = x;
    ASSERT_TRUE(w == x && w1 == x);
    BigInt d(move(y));
    ASSERT_TRUE(d.digits() == a && d.sign_() == -1 && y.digits().empty());
}

TEST(BigintTest, test_add) {
    BigInt x(9), y(1);
    ASSERT_EQ(x + y, BigInt(10));
    ASSERT_EQ(x + BigInt(0), BigInt(9));
    ASSERT_EQ(x + BigInt(1234115), BigInt(1234124));
    ASSERT_EQ(x + BigInt("-9"), BigInt(0));
    x += y;
    ASSERT_EQ(x, BigInt(10));
}

TEST(BigintTest, test_substr) {
    BigInt x(9), y(1), z(-9);
    ASSERT_EQ(x - y, BigInt(8));
    ASSERT_EQ(x - BigInt(0), BigInt(9));
    ASSERT_EQ(BigInt(1234115) - x, BigInt(1234106));
    ASSERT_EQ(z + BigInt("-9"), BigInt(-18));
    ASSERT_EQ(z - BigInt("-9"), BigInt(0));
    x -= y;
    ASSERT_EQ(x, BigInt(8));
}

TEST(BigintTest, test_multiply) {
    BigInt x(9), y(1), z(-9), w(6);
    ASSERT_EQ(x * y, BigInt(9));
    ASSERT_EQ(x * z, BigInt(-81));
    ASSERT_EQ(z * z, BigInt(81));
    ASSERT_EQ(x * BigInt(0), BigInt(0));
    x *= w;
    ASSERT_EQ(x, BigInt(54));
}

TEST(BigintTest, test_division) {
    BigInt x(9), k(3), w(81), d(1);
    ASSERT_TRUE((k / x) == BigInt(0));
    ASSERT_THROW(k / BigInt(0), std::runtime_error);
    ASSERT_EQ(x / k, BigInt(3));
    ASSERT_EQ(w / x, BigInt(9));
    ASSERT_EQ(w / (-x), BigInt(-9));
    ASSERT_EQ(k / d, BigInt(3));
    x /= k;
    ASSERT_EQ(x, BigInt(3));
}

TEST(BigintTest, test_input) {
    BigInt x, y;
    std::stringstream ss, ss1;
    ss << "224";
    ss1 << "awrf";
    ss >> x;
    ASSERT_EQ(x, BigInt(224));
    ASSERT_THROW(ss1 >> y, std::invalid_argument);
}

TEST(BigintTest, test_output) {
    BigInt x(224);
    std::stringstream ss;
    ss << x;
    ASSERT_EQ(ss.str(), "224");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}