#include <gtest/gtest.h>
#include "my_lib.hpp"
#include <exception>
#include <iostream>
using namespace std;

TEST(BigintTest, test_fft1) {
    BigInt x(8), y(2);
    BigInt z = x.fft_multiply(y);
    ASSERT_EQ(z, BigInt(16));
}

TEST(BigintTest, test_fft2) {
    BigInt x("00989912"), y(0);
    BigInt z = x.fft_multiply(y);
    ASSERT_EQ(z, BigInt(0));
}

TEST(BigintTest, test_fft3) {
    BigInt x(11), y(-11);
    BigInt z = x.fft_multiply(y);
    ASSERT_EQ(z, BigInt("-121"));
}

TEST(BigintTest, test_fft4) {
    BigInt x(-11234), y(-34567);
    BigInt z = x.fft_multiply(y);
    ASSERT_EQ(z, BigInt(388325678));
}

TEST(BigintTest, test_fft5) {
    BigInt x(123), y(-45);
    BigInt z = x.fft_multiply(y);
    ASSERT_EQ(z, BigInt("-5535"));
}

TEST(BigintTest, test_fft6) {
    BigInt x(672134), y(4545);
    BigInt z = x.fft_multiply(y);
    ASSERT_EQ(z, BigInt(3054849030));
}

TEST(BigintTest, test_fft7) {
    BigInt x("12345678987654321"), y("12345678987654321");
    BigInt z = x.fft_multiply(y);
    ASSERT_EQ(z, BigInt("152415789666209420210333789971041"));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}