#include <gtest/gtest.h>
#include "my_lib.hpp"
#include <exception>
#include <iostream>
using namespace std;
using namespace my_container;

TEST(ArrayTest, test_init_list) {
    Array<int, 3> a{ 0, 1, 2 };
    ASSERT_EQ(a.size(), 3);
    for (size_t i = 0; i < a.size(); i++)
        ASSERT_EQ(a[i], i);
}

TEST(ArrayTest, test_copy_constr) {
    Array<int, 3> a{ 0, 1, 2 };
    Array<int, 3> b(a);
    ASSERT_TRUE(a == b);
    for (size_t i = 0; i < a.size(); i++)
        ASSERT_EQ(b[i], a[i]);
}

TEST(ArrayTest, eq_test) {
    Array<int, 3> a{ 0, 1, 2 };
    Array<int, 3> b{ 0, 1, 2 };
    Array<int, 3> c{ 1, 1, 2 };
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

TEST(ArrayTest, test_size) {
    Array<int, 3> a;
    //Array<int, 0> b;
    ASSERT_EQ(a.size(), 3);
    ASSERT_EQ(a.max_size(), 3);
    //ASSERT_TRUE(b.empty());
}

TEST(ArrayTest, test_fill) {
    Array<int, 5> a = { 1, 2, 3, 4, 5 };
    a.fill(3);
    for (size_t i = 0; i < a.size(); i++)
        ASSERT_EQ(a[i], 3);
}

TEST(ArrayTest, test_swap) {
    Array<int, 3> a = { 1, 1, 1 };
    Array<int, 3> b = { 2, 2, 2 };
    a.swap(b);
    for (size_t i = 0; i < a.size(); i++) {
        ASSERT_EQ(a[i], 2);
        ASSERT_EQ(b[i], 1);
    }
}

TEST(ArrayTest, test_data) {
    Array<int, 5> a = { 1, 2, 3 };
    int* pti = a.data();
    ASSERT_EQ(*pti, 1);
    ASSERT_EQ(*(pti + 1), 2);
    ASSERT_EQ(*(pti + 2), 3);
}

TEST(ArrayTest, test_oper_eq) {
    Array<int, 3> a{ 1, 2, 3 };
    Array<int, 3> b = a;
    ASSERT_TRUE(a == b);
    Array<int, 3> c{ 2, 3, 5 };
    ASSERT_TRUE(b != c);
}

TEST(ArrayTest, test_front_back) {
    Array<int, 5> a = { 1, 2, 3, 4, 5 };
    ASSERT_EQ(a.front(), 1);
    ASSERT_EQ(a.back(), 5);
    a.front() = 10;
    ASSERT_EQ(a[0], 10);
    Array<int, 5> b = a;
    ASSERT_EQ(b.front(), 10);
    ASSERT_EQ(b.back(), 5);
}

TEST(ArrayTest, test_at) {
    Array<int, 2> a{ 5, 8 };
    ASSERT_EQ(a[0], 5);
    ASSERT_EQ(a[1], 8);
    ASSERT_EQ(a.at(0), 5);
    ASSERT_EQ(a.at(1), 8);
}

TEST(ArrayTest, test_out) {
    Array<int, 5> a = { 1, 2, 3, 4, 5 };
    ASSERT_EQ(a[0], 1);
    ASSERT_THROW(a.at(5), std::out_of_range);
    ASSERT_THROW(a.at(-1), std::out_of_range);
}

TEST(ArrayTest, test_iter) {
    Array<int, 5> a = { 0, 1, 2, 3, 4 };
    int i = 0;
    for (auto it = a.begin(); it < a.end(); it++) {
        ASSERT_EQ(a[i], *it);
        i++;
    }
}

TEST(ArrayTest, test_const_iter) {
    Array<int, 5> a = { 0, 1, 2, 3, 4 };
    int i = a.size() - 1;
    for (auto it = a.cend() - 1; it >= a.cbegin(); it--) {
        ASSERT_EQ(a[i], *it);
        i--;
    }
}

TEST(ArrayTest, test_riter) {
    Array<int, 5> a = { 0, 1, 2, 3, 4 };
    int i = a.size() - 1;
    for (auto it = a.rbegin(); it < a.rend(); it++) {
        ASSERT_EQ(a[i], *it);
        i--;
    }
}

TEST(ArrayTest, test_const_riter) {
    Array<int, 5> a = { 0, 1, 2, 3, 4 };
    int i = 0;
    for (auto it = a.crend() - 1; it >= a.crbegin(); it--) {
        ASSERT_EQ(a[i], *it);
        i++;
    }
}

TEST(ArrayTest, test_move_constructor) {
    Array<int, 3> a{ 1, 2, 3 };
    Array<int, 3> b(std::move(a));
    ASSERT_EQ(b[0], 1);
    ASSERT_EQ(b[1], 2);
    ASSERT_EQ(b[2], 3);
}

TEST(ArrayTest, test_move_assignment) {
    Array<int, 3> a{ 1, 2, 3 };
    Array<int, 3> b(std::move(a));
    ASSERT_EQ(b[0], 1);
    ASSERT_EQ(b[1], 2);
    ASSERT_EQ(b[2], 3);
}

TEST(ArrayTest, test_iterator_operators) {
    Array<int, 3> a{ 1, 2, 3 };
    auto it1 = a.begin();
    auto it2 = a.begin();

    ++it2;
    ASSERT_EQ(*it2, 2);
    ASSERT_TRUE(it1 != it2);

    auto it3 = it1++;
    ASSERT_EQ(*it1, 2);
    ASSERT_EQ(*it3, 1);

    --it1;
    ASSERT_EQ(*it1, 1);

    auto it4 = it2--;
    ASSERT_EQ(*it2, 1);
    ASSERT_EQ(*it4, 2);

    ASSERT_TRUE(it1 == it2);
    ASSERT_FALSE(it1 != it2);
}

TEST(ArrayTest, test_reverse_iterator_operators) {
    Array<int, 3> a{ 1, 2, 3 };
    auto rit1 = a.rbegin();
    auto rit2 = a.rbegin();

    ++rit2;
    ASSERT_EQ(*rit2, 2);
    ASSERT_TRUE(rit1 != rit2);

    auto rit3 = rit1++;
    ASSERT_EQ(*rit1, 2);
    ASSERT_EQ(*rit3, 3);

    --rit1;
    ASSERT_EQ(*rit1, 3);

    auto rit4 = rit2--;
    ASSERT_EQ(*rit2, 3);
    ASSERT_EQ(*rit4, 2);

    ASSERT_TRUE(rit1 == rit2);
    ASSERT_FALSE(rit1 != rit2);
}

TEST(ArrayTest, test_iterator_loop) {
    Array<int, 5> a = { 10, 20, 30, 40, 50 };

    int i = 0;
    int expected[] = { 10, 20, 30, 40, 50 };
    for (auto it = a.begin(); it != a.end(); ++it) {
        ASSERT_EQ(*it, expected[i++]);
    }

    i = 4;
    for (auto rit = a.rbegin(); rit != a.rend(); ++rit) {
        ASSERT_EQ(*rit, expected[i--]);
    }
}

TEST(ArrayTest, test_const_iterator) {
    const Array<int, 3> a{ 1, 2, 3 };

    int sum = 0;
    for (auto it = a.cbegin(); it != a.cend(); ++it) {
        sum += *it;
    }
    ASSERT_EQ(sum, 6);

    sum = 0;
    for (auto rit = a.crbegin(); rit != a.crend(); ++rit) {
        sum += *rit;
    }
    ASSERT_EQ(sum, 6);
}

TEST(ArrayTest, test_iterator_edge_cases) {
    Array<int, 1> a{ 42 };

    auto it = a.begin();
    ASSERT_EQ(*it, 42);
    ++it;
    ASSERT_EQ(it, a.end());

    auto rit = a.rbegin();
    ASSERT_EQ(*rit, 42);
    ++rit;
    ASSERT_EQ(rit, a.rend());
}

TEST(ArrayTest, test_iterator_comparison) {
    Array<int, 4> a{ 1, 2, 3, 4 };

    auto it1 = a.begin();
    auto it2 = a.begin() + 1;

    ASSERT_TRUE(it2 > it1);
    ASSERT_TRUE(it1 < it2);
    ASSERT_TRUE(it2 >= it1);
    ASSERT_TRUE(it1 <= it2);

    ASSERT_TRUE(it1 != it2);
    ASSERT_FALSE(it1 == it2);

    ++it1;
    ASSERT_TRUE(it1 == it2);
    ASSERT_TRUE(it1 >= it2);
    ASSERT_TRUE(it1 <= it2);
    ASSERT_FALSE(it1 != it2);

    auto rit1 = a.rbegin();
    auto rit2 = a.rbegin() + 1;
    ASSERT_TRUE(rit2 < rit1);
    ASSERT_TRUE(rit1 > rit2);
    ASSERT_TRUE(rit2 <= rit1);
    ASSERT_TRUE(rit1 >= rit2);

    ASSERT_TRUE(rit1 != rit2);
    ASSERT_FALSE(rit1 == rit2);

    ++rit1;
    ASSERT_TRUE(rit1 == rit2);
    ASSERT_FALSE(rit1 != rit2);
}

TEST(ArrayTest, test_empty) {
    Array<int, 4> a{ 1, 2, 3, 4 };
    ASSERT_FALSE(a.empty());
}

TEST(ArrayTest, test_assignment_operator) {
    Array<int, 3> a{ 1, 2, 3 };
    Array<int, 3> b = a;
    ASSERT_EQ(b[0], 1);
    ASSERT_EQ(b[1], 2);
    ASSERT_EQ(b[2], 3);
}

TEST(ArrayTest, test_all_comparisons) {
    Array<int, 3> a{ 1, 2, 3 };
    Array<int, 3> b{ 1, 2, 4 };
    Array<int, 3> c{ 1, 2, 3 };

    ASSERT_TRUE(a <= b);
    ASSERT_TRUE(a <= c);
    ASSERT_TRUE(b >= a);
    ASSERT_TRUE(c >= a);
    ASSERT_FALSE(a > c);
    ASSERT_FALSE(a < c);
}

TEST(ArrayTest, test_iterator_arithmetic) {
    Array<int, 5> a{ 1, 2, 3, 4, 5 };
    auto it = a.begin();

    ASSERT_EQ(*(it + 2), 3);
    ASSERT_EQ(*(it + 4), 5);
    ASSERT_EQ(it + 5, a.end());
    it++;
    ASSERT_EQ(*(it - 1), 1);

    auto rit = a.rbegin();
    ASSERT_EQ(*(rit + 2), 3);
    ASSERT_EQ(*(rit + 4), 1);
    ASSERT_EQ(rit + 5, a.rend());
    rit++;
    ASSERT_EQ(*(rit - 1), 5);
}

TEST(ArrayTest, test_eq2) {
    Array<int, 5> a{ 1, 2, 3, 4, 5 };
    Array<int, 5> b = a;
    ASSERT_EQ(b, a);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}