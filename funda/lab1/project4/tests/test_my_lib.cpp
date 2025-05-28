#include <gtest/gtest.h>
#include "my_lib.hpp"
#include <exception>
#include <iostream>
using namespace std;
using namespace my_container;

TEST(StackTest, test_top) {
    Stack<int> a{ 0, 1, 2 };
    ASSERT_EQ(a.top(), 2);
    ASSERT_EQ(a.size(), 3);
    Stack<int> b(a);
    b.push(3);
    ASSERT_EQ(b.top(), 3);
}

TEST(StackTest, test_eq) {
    Stack<int> a{ 0, 1, 2 };
    Stack<int> b{ 0, 1 };
    Stack<int> c = a;
    ASSERT_EQ(a == b, false);
    ASSERT_TRUE(a == c);
    ASSERT_TRUE(a != b);
    ASSERT_TRUE(a > b);
    ASSERT_TRUE(a >= b);
    ASSERT_TRUE(b < a);
    ASSERT_TRUE(b <= a);
    ASSERT_EQ(a <=> b, std::strong_ordering::greater);
    ASSERT_EQ(a <=> c, std::strong_ordering::equal);
}

TEST(StackTest, test_swap) {
    Stack<int> a = { 1, 1, 1 };
    Stack<int> b = { 2, 2, 2, 2 };
    a.swap(b);
    ASSERT_EQ(a.size(), 4);
    ASSERT_EQ(b.size(), 3);
    while (!b.empty()) {
        ASSERT_EQ(b.top(), 1);
        b.pop();
    }
}

TEST(StackTest, constr) {
    Stack<int> a;
    ASSERT_TRUE(a.empty());
    Stack<int> b{ 0, 1, 2 };
    ASSERT_TRUE(b.size() == 3);
    for (int i = 2; i >= 0; i--) {
        ASSERT_EQ(b.top(), i);
        b.pop();
    }
    b.push(0);
    b.push(1);
    b.push(2);
    Stack<int> c(b);
    ASSERT_TRUE(c == b);
    Stack<int> d(move(c));
    ASSERT_EQ(d.size(), 3);
    ASSERT_EQ(d.top(), 2);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}