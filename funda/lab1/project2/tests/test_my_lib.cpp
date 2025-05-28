#include <gtest/gtest.h>
#include "my_lib.hpp"

using namespace my_container;
using namespace std;

TEST(ListTest, DefaultConstructor) {
    List<int> list;
    EXPECT_EQ(list.size(), 0);
    EXPECT_TRUE(list.empty());
}

TEST(ListTest, InitializerListConstructor) {
    List<int> list = { 1, 2, 3, 4 };
    EXPECT_EQ(list.size(), 4);
    EXPECT_FALSE(list.empty());
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.back(), 4);
}

TEST(ListTest, CopyListConstructor) {
    List<int> list = { 1, 2, 3, 4 };
    List<int> clist = list;
    EXPECT_TRUE(list == clist);
}

TEST(ListTest, PushBack) {
    List<int> list;
    list.push_back(1);
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.back(), 1);

    list.push_back(2);
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.back(), 2);
}

TEST(ListTest, PushFront) {
    List<int> list;
    list.push_front(1);
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.front(), 1);

    list.push_front(2);
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.front(), 2);
}

TEST(ListTest, PopBack) {
    List<int> list = { 1, 2, 3 };
    list.pop_back();
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.back(), 2);

    list.pop_back();
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.back(), 1);
}

TEST(ListTest, PopFront) {
    List<int> list = { 1, 2, 3 };
    list.pop_front();
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.front(), 2);

    list.pop_front();
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.front(), 3);
}

TEST(ListTest, Insert) {
    List<int> list = { 1, 3 };
    auto it = list.begin();
    ++it;
    list.insert(it, 2);

    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.back(), 3);

    auto check = list.begin();
    ++check;
    EXPECT_EQ(*check, 2);
}

TEST(ListTest, Erase) {
    List<int> list = { 1, 2, 3 };
    auto it = list.begin();
    ++it;
    list.erase(it);

    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.back(), 3);
}

TEST(ListTest, Clear) {
    List<int> list = { 1, 2, 3 };
    list.clear();
    EXPECT_EQ(list.size(), 0);
    EXPECT_TRUE(list.empty());
}

TEST(ListTest, Iterator) {
    List<int> list = { 1, 2, 3 };
    int sum = 0;
    for (auto it = list.begin(); it != list.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 6);
}

TEST(ListTest, ConstIterator) {
    const List<int> list = { 1, 2, 3 };
    int sum = 0;
    for (auto it = list.cbegin(); it != list.cend(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 6);
}

TEST(ListTest, ReverseIterator) {
    List<int> list = { 1, 2, 3 };
    std::vector<int> reversed;
    for (auto it = list.rbegin(); it != list.rend(); ++it) {
        reversed.push_back(*it);
    }
    EXPECT_EQ(reversed, std::vector<int>({ 3, 2, 1 }));
}

TEST(ListTest, ComparisonOperators) {
    List<int> list1 = { 1, 2, 3 };
    List<int> list2 = { 1, 2, 3 };
    List<int> list3 = { 1, 2 };
    List<int> list4 = { 1, 2, 4 };

    EXPECT_TRUE(list1 == list2);
    EXPECT_FALSE(list1 == list3);
    EXPECT_TRUE(list1 != list3);
    EXPECT_TRUE(list3 < list1);
    EXPECT_TRUE(list1 > list3);
    EXPECT_TRUE(list1 <= list2);
    EXPECT_TRUE(list1 >= list2);
    EXPECT_TRUE(list1 < list4);
}

TEST(ListTest, Resize) {
    List<int> list = { 1, 2, 3 };
    list.resize(2);
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.back(), 2);

    list.resize(4, 5);
    EXPECT_EQ(list.size(), 4);
    EXPECT_EQ(list.back(), 5);
}

TEST(ListTest, Swap) {
    List<int> list1 = { 1, 2, 3 };
    List<int> list2 = { 4, 5, 6 };

    List<int>::swap(list1, list2);

    EXPECT_EQ(list1.front(), 4);
    EXPECT_EQ(list2.front(), 1);
}

TEST(ListTest, EdgeCases) {
    List<int> emptyList;
    EXPECT_THROW(emptyList.front(), std::out_of_range);
    EXPECT_THROW(emptyList.back(), std::out_of_range);

    List<int> singleElement = { 42 };
    EXPECT_EQ(singleElement.front(), 42);
    EXPECT_EQ(singleElement.back(), 42);

    singleElement.pop_front();
    EXPECT_TRUE(singleElement.empty());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}