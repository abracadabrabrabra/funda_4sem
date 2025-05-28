#include <gtest/gtest.h>
#include "my_lib.hpp"
#include <exception>
#include <iostream>
using namespace std;
using namespace my_container;

TEST(VectorTest, test_init_list) {
    Vector<int> v{ 0, 1, 2 };
    ASSERT_EQ(v.size(), 3);
    ASSERT_EQ(v.capacity(), 3);
    for (size_t i = 0; i < v.size(); i++)
        ASSERT_EQ(v[i], i);
}

TEST(VectorTest, test_copy_constr) {
    Vector<int> a{ 0, 1, 2 };
    Vector<int> b(a);
    ASSERT_TRUE(a == b);
    for (size_t i = 0; i < a.size(); i++)
        ASSERT_EQ(b[i], a[i]);
}

TEST(VectorTest, test_assignment) {
    Vector<int> a{ 0, 1, 2 };
    Vector<int> b = a;
    ASSERT_TRUE(a == b);
    for (size_t i = 0; i < a.size(); i++)
        ASSERT_EQ(b[i], a[i]);
}

TEST(VectorTest, default_constr) {
    Vector<int> a;
    ASSERT_TRUE(!a.size() && !a.capacity() && !a.data());
}

TEST(VectorTest, test_move) {
    Vector<int> v{ 0, 1, 2 };
    Vector<int> mv(move(v));
    ASSERT_EQ(mv.size(), 3);
    ASSERT_EQ(mv.capacity(), 3);
    for (size_t i = 0; i < mv.size(); i++)
        ASSERT_EQ(mv[i], i);
}


TEST(VectorTest, compare_test) {
    Vector<int> a{ 0, 1, 2 };
    Vector<int> b{ 0, 1, 2 };
    Vector<int> c{ 1, 1, 2 };
    ASSERT_EQ(a == b, true);
    ASSERT_EQ(a != b, false);
    ASSERT_EQ(a == c, false);
    ASSERT_EQ(a != c, true);
    ASSERT_TRUE(a < c);
    ASSERT_TRUE(c > b);
    ASSERT_TRUE(a <= c);
    ASSERT_TRUE(c >= b);
    ASSERT_EQ(a <=> b, std::strong_ordering::equal);
    ASSERT_EQ(a <=> c, std::strong_ordering::less);
}

TEST(VectorTest, test_swap) {
    Vector<int> a{ 1, 1, 1 };
    Vector<int> b{ 2, 2, 2 };
    a.swap(b);
    for (size_t i = 0; i < a.size(); i++) {
        ASSERT_EQ(a[i], 2);
        ASSERT_EQ(b[i], 1);
    }
}

TEST(VectorTest, test_access) {
    Vector<int> a{ 1, 2, 3 };
    ASSERT_EQ(a.front(), 1);
    ASSERT_EQ(a.back(), 3);
    ASSERT_EQ(a[1], 2);
    ASSERT_EQ(a.at(1), 2);
    ASSERT_THROW(a.at(100), std::out_of_range);
    int* pti = a.data();
    ASSERT_EQ(*pti, 1);
    ASSERT_EQ(*(pti + 1), 2);
    ASSERT_EQ(*(pti + 2), 3);
}

TEST(VectorTest, memory_control_test) {
    Vector<int> a{ 1, 2, 3 };
    a.reserve(10);
    ASSERT_EQ(a.size(), 3);
    ASSERT_EQ(a.capacity(), 10);
    a.reserve(2);
    ASSERT_EQ(a.size(), 3);
    ASSERT_EQ(a.capacity(), 10);

    Vector<int> b{ 1, 2, 3 };
    b.reserve(12);
    ASSERT_EQ(b.capacity(), 12);
    b.shrink_to_fit();
    ASSERT_TRUE(b.size() == b.capacity());
}

TEST(VectorTest, push_pop_test) {
    Vector<int> a{ 1, 2, 3 };
    Vector<int> b{ 1, 2 };
    a.push_back(10);
    ASSERT_TRUE(a.size() == 4 && a.back() == 10);
    a.pop_back();
    a.pop_back();
    ASSERT_TRUE(a == b);
}

TEST(VectorTest, clear_test) {
    Vector<int> a{ 1, 2, 3 };
    a.clear();
    ASSERT_TRUE(!a.size() && a.capacity() == 3);
    a.pop_back();
    a.pop_back();
    a.push_back(1);
    a.push_back(45);
    ASSERT_TRUE(a.size() == 2 && a.front() == 1 && a.back() == 45);
}

TEST(VectorTest, resize_test) {
    Vector<int> a{ 1, 2, 3 };
    a.resize(5);
    ASSERT_TRUE(a.size() == 5 && a.capacity() == 5);
    a.resize(1);
    ASSERT_TRUE(a.size() == 1 && a.capacity() == 5);
}

TEST(VectorTest, insert_test) {
    Vector<int> a{ 1, 2, 3 }, b{ 1, 45, 2, 3 }, c{ 3, 3, 1, 45, 2, 3 };
    auto it = a.cbegin() + 1;
    a.insert(it, 45);
    ASSERT_TRUE(a == b);
    it = a.cbegin();
    a.insert(it, 2, 3);
    ASSERT_TRUE(a == c);
}

TEST(VectorTest, erase_test) {
    Vector<int> a{ 1, 2, 3 }, b{ 3, 3, 1, 2, 3 }, c{ 3, 3, 1, 45, 2, 3 };
    auto cit = c.cbegin() + 3;
    c.erase(cit);
    ASSERT_TRUE(c == b);
    auto it = c.begin();
    c.erase(it, it + 2);
    ASSERT_TRUE(c == a);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}