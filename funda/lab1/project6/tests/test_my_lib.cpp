#include <gtest/gtest.h>
#include "my_lib.hpp"
#include <exception>
#include <iostream>
using namespace std;
using namespace my_smart_ptr;

TEST(PtrTest, test_default_constr) {
    UniquePtr<int> p1;
    UniquePtr<int[]> p2;
    ASSERT_TRUE(!p1.get());
    ASSERT_TRUE(!p2.get());
}

TEST(PtrTest, test_pointer_constr) {
    UniquePtr<int> p1(new int(3));
    ASSERT_EQ(*p1, 3);
    int* pti = new int[3];
    *pti = 0;
    *(pti + 1) = 1;
    *(pti + 2) = 2;
    UniquePtr<int[]> p2(pti);
    ASSERT_EQ(p2[0], 0);
    ASSERT_EQ(p2[1], 1);
    ASSERT_EQ(p2[2], 2);
    ASSERT_EQ(*p2, 0);
}

TEST(PtrTest, test_move) {
    UniquePtr<int> p1(new int(3));
    UniquePtr<int> p2(move(p1));
    ASSERT_EQ(*p2, 3);
    ASSERT_TRUE(!p1.get());
    UniquePtr<int> p3 = move(p2);
    ASSERT_EQ(*p3, 3);
    ASSERT_TRUE(!p2.get());
}

TEST(PtrTest, test_move_arr) {
    int* pti = new int[3];
    *pti = 0;
    *(pti + 1) = 1;
    *(pti + 2) = 2;
    UniquePtr<int[]> p1(pti);
    UniquePtr<int[]> p2(move(p1));
    ASSERT_EQ(*p2, 0);
    ASSERT_TRUE(!p1.get());
    UniquePtr<int[]> p3 = move(p2);
    ASSERT_EQ(*p3, 0);
    ASSERT_TRUE(!p2.get());
}

TEST(PtrTest, test_bool) {
    UniquePtr<int> p1(new int(3));
    bool is_valid1 = static_cast<bool>(p1);
    ASSERT_TRUE(is_valid1);
    UniquePtr<int[]> p2;
    bool is_valid2 = static_cast<bool>(p2);
    ASSERT_TRUE(!is_valid2);
}

TEST(PtrTest, test_release) {
    UniquePtr<int> p1(new int(3));
    int* pti = p1.release();
    ASSERT_TRUE(*pti == 3);
    ASSERT_TRUE(!p1.get());
    delete pti;

    UniquePtr<int[]> p2(new int[3]);
    int* pti2 = p2.release();
    ASSERT_TRUE(!p2.get());
    delete[] pti2;
}

TEST(PtrTest, test_reset) {
    UniquePtr<int> p1(new int(3));
    int* pti{ new int(5) };
    p1.reset(pti);
    ASSERT_TRUE(*p1 == 5);

    UniquePtr<int[]> p3(new int[3]);
    p3.reset();
    ASSERT_TRUE(!p3.get());
}

TEST(PtrTest, test_swap) {
    UniquePtr<int> p1(new int(3));
    UniquePtr<int> p2(new int(4));
    p1.swap(p2);
    ASSERT_EQ(*p1, 4);
    ASSERT_EQ(*p2, 3);

    int* pt1 = new int[3];
    *pt1 = 0;
    int* pt2 = new int[2];
    *pt2 = 1;
    UniquePtr<int[]> p3(pt1);
    UniquePtr<int[]> p4(pt2);
    p3.swap(p4);
    ASSERT_EQ(*p3, 1);
    ASSERT_EQ(*p4, 0);

}

TEST(PtrTest, test_pointer) {
    struct my_struct{
        int val1 = 34;
        double val2 = 1.4;
    };
    UniquePtr<my_struct> p(new my_struct());
    ASSERT_EQ(p->val1, 34);
    ASSERT_EQ(p->val2, 1.4);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}