#include <gtest/gtest.h>
#include "my_lib.hpp"
#include <exception>
#include <utility>
#include <iostream>
#include <functional>
using namespace std;

TEST(BSTTest, DefaultConstructor) {
    BST<int, string> tree;
    EXPECT_EQ(tree.size(), 0);
    EXPECT_FALSE(tree.contains(1));
}

TEST(BSTTest, InsertAndFind) {
    BST<int, string> tree;
    tree.insert(5, "five");
    tree.insert(3, "three");
    tree.insert(7, "seven");
    EXPECT_EQ(tree.size(), 3);
    auto* val = tree.find(5);
    ASSERT_NE(val, nullptr);
    EXPECT_EQ(*val, "five");
    val = tree.find(3);
    ASSERT_NE(val, nullptr);
    EXPECT_EQ(*val, "three");
    val = tree.find(7);
    ASSERT_NE(val, nullptr);
    EXPECT_EQ(*val, "seven");
    EXPECT_EQ(tree.find(10), nullptr);
}

TEST(BSTTest, InsertDuplicate) {
    BST<int, string> tree;
    tree.insert(5, "five");
    tree.insert(5, "new five");
    EXPECT_EQ(tree.size(), 1);
    auto* val = tree.find(5);
    ASSERT_NE(val, nullptr);
    EXPECT_EQ(*val, "new five");
}

TEST(BSTTest, Remove) {
    BST<int, string> tree;
    tree.insert(5, "five");
    tree.insert(3, "three");
    tree.insert(7, "seven");
    tree.insert(2, "two");
    tree.insert(4, "four");
    tree.insert(6, "six");
    tree.insert(8, "eight");
    EXPECT_EQ(tree.size(), 7);
    tree.remove(2);
    EXPECT_EQ(tree.size(), 6);
    EXPECT_FALSE(tree.contains(2));
    tree.remove(3);
    EXPECT_EQ(tree.size(), 5);
    EXPECT_FALSE(tree.contains(3));
    EXPECT_TRUE(tree.contains(4));
    tree.remove(5);
    EXPECT_EQ(tree.size(), 4);
    EXPECT_FALSE(tree.contains(5));
    EXPECT_TRUE(tree.contains(6));
    EXPECT_TRUE(tree.contains(7));
    EXPECT_TRUE(tree.contains(4));
    EXPECT_TRUE(tree.contains(8));
    tree.remove(100);
    EXPECT_EQ(tree.size(), 4);
}

TEST(BSTTest, Clear) {
    BST<int, string> tree;
    tree.insert(5, "five");
    tree.insert(3, "three");
    tree.insert(7, "seven");
    tree.clear();
    EXPECT_EQ(tree.size(), 0);
    EXPECT_FALSE(tree.contains(5));
    EXPECT_FALSE(tree.contains(3));
    EXPECT_FALSE(tree.contains(7));
}

TEST(BSTTest, Traversals) {
    BST<int, string> tree;
    tree.insert(4, "four");
    tree.insert(2, "two");
    tree.insert(6, "six");
    tree.insert(1, "one");
    tree.insert(3, "three");
    tree.insert(5, "five");
    tree.insert(7, "seven");

    vector<pair<int, string>> inorder_result;
    auto inorder_cb = [&](const int& k, const string& v) {
        inorder_result.emplace_back(k, v);
        };
    tree.inorder(inorder_cb);

    vector<pair<int, string>> expected_inorder = {
        {1, "one"}, {2, "two"}, {3, "three"}, {4, "four"},
        {5, "five"}, {6, "six"}, {7, "seven"}
    };
    EXPECT_EQ(inorder_result, expected_inorder);

    vector<pair<int, string>> prefix_result;
    auto prefix_cb = [&](const int& k, const string& v) {
        prefix_result.emplace_back(k, v);
        };
    tree.prefix(prefix_cb);

    vector<pair<int, string>> expected_prefix = {
        {4, "four"}, {2, "two"}, {1, "one"}, {3, "three"},
        {6, "six"}, {5, "five"}, {7, "seven"}
    };
    EXPECT_EQ(prefix_result, expected_prefix);

    vector<pair<int, string>> postorder_result;
    auto postorder_cb = [&](const int& k, const string& v) {
        postorder_result.emplace_back(k, v);
        };
    tree.postorder(postorder_cb);

    vector<pair<int, string>> expected_postorder = {
        {1, "one"}, {3, "three"}, {2, "two"}, {5, "five"},
        {7, "seven"}, {6, "six"}, {4, "four"}
    };
    EXPECT_EQ(postorder_result, expected_postorder);
}

TEST(BSTTest, Iterator) {
    BST<int, string> tree;
    tree.insert(4, "four");
    tree.insert(2, "two");
    tree.insert(6, "six");
    tree.insert(1, "one");
    tree.insert(3, "three");
    tree.insert(5, "five");
    tree.insert(7, "seven");
    vector<pair<int, string>> result;
    for (auto it = tree.begin(); it != tree.end(); ++it) {
        result.emplace_back((*it).get_key(), (*it).get_val());
    }
    vector<pair<int, string>> expected = {
        {1, "one"}, {2, "two"}, {3, "three"}, {4, "four"},
        {5, "five"}, {6, "six"}, {7, "seven"}
    };
    EXPECT_EQ(result, expected);
    BST<int, string> empty_tree;
    EXPECT_EQ(empty_tree.begin(), empty_tree.end());
}

TEST(BSTTest, CopyAndMove) {
    BST<int, string> tree;
    tree.insert(4, "four");
    tree.insert(2, "two");
    tree.insert(6, "six");
    BST<int, string> tree_copy = tree;
    EXPECT_EQ(tree_copy.size(), 3);
    EXPECT_TRUE(tree_copy.contains(4));
    EXPECT_TRUE(tree_copy.contains(2));
    EXPECT_TRUE(tree_copy.contains(6));
    tree.insert(5, "five");
    EXPECT_EQ(tree.size(), 4);
    EXPECT_EQ(tree_copy.size(), 3);
    EXPECT_FALSE(tree_copy.contains(5));
    BST<int, string> tree_moved = move(tree);
    EXPECT_EQ(tree_moved.size(), 4);
    EXPECT_TRUE(tree_moved.contains(5));
    EXPECT_EQ(tree.size(), 0);
    EXPECT_EQ(tree.begin(), tree.end());
}

TEST(BSTTest, CustomComparator) {
    BST<int, string, GreaterComparator> tree;
    tree.insert(5, "five");
    tree.insert(3, "three");
    tree.insert(7, "seven");
    vector<pair<int, string>> result;
    for (auto it = tree.begin(); it != tree.end(); ++it) {
        result.emplace_back((*it).get_key(), (*it).get_val());
    }
    vector<pair<int, string>> expected = {
        {7, "seven"}, {5, "five"}, {3, "three"}
    };
    EXPECT_EQ(result, expected);
}

TEST(MyMapTest, BasicOperations) {
    MyMap<int, string> map;
    map.insert(1, "one");
    map.insert(2, "two");
    map.insert(3, "three");
    EXPECT_EQ(map.size(), 3);
    EXPECT_TRUE(map.contains(1));
    EXPECT_TRUE(map.contains(2));
    EXPECT_TRUE(map.contains(3));
    EXPECT_FALSE(map.contains(4));
    auto* val = map.find(2);
    ASSERT_NE(val, nullptr);
    EXPECT_EQ(*val, "two");
    map.remove(2);
    EXPECT_EQ(map.size(), 2);
    EXPECT_FALSE(map.contains(2));
}

TEST(MyMapTest, Iterator) {
    MyMap<int, string> map;
    map.insert(3, "three");
    map.insert(1, "one");
    map.insert(2, "two");
    vector<pair<int, string>> result;
    for (auto it = map.begin(); it != map.end(); ++it) {
        result.emplace_back((*it).get_key(), (*it).get_val());
    }
    vector<pair<int, string>> expected = {
        {1, "one"}, {2, "two"}, {3, "three"}
    };
    EXPECT_EQ(result, expected);
}

TEST(MyMapTest, Traversals) {
    MyMap<int, string> map;
    map.insert(2, "two");
    map.insert(1, "one");
    map.insert(3, "three");
    vector<pair<int, string>> inorder_result;
    auto inorder_cb = [&](const int& k, const string& v) {
        inorder_result.emplace_back(k, v);
        };
    map.inorder(inorder_cb);
    vector<pair<int, string>> expected_inorder = {
        {1, "one"}, {2, "two"}, {3, "three"}
    };
    EXPECT_EQ(inorder_result, expected_inorder);
}

TEST(MyMapTest, CopyAndMove) {
    MyMap<int, string> map;
    map.insert(1, "one");
    map.insert(2, "two");
    MyMap<int, string> map_copy = map;
    EXPECT_EQ(map_copy.size(), 2);
    EXPECT_TRUE(map_copy.contains(1));
    EXPECT_TRUE(map_copy.contains(2));
    map.insert(3, "three");
    EXPECT_EQ(map.size(), 3);
    EXPECT_EQ(map_copy.size(), 2);
    EXPECT_FALSE(map_copy.contains(3));
    MyMap<int, string> map_moved = move(map);
    EXPECT_EQ(map_moved.size(), 3);
    EXPECT_TRUE(map_moved.contains(3));
    EXPECT_EQ(map.size(), 0);
    EXPECT_EQ(map.begin(), map.end());
}

TEST(MyMapTest, CustomComparator) {
    MyMap<int, string, BST, GreaterComparator> map;
    map.insert(1, "one");
    map.insert(3, "three");
    map.insert(2, "two");
    vector<pair<int, string>> result;
    auto cb = [&](const int& k, const string& v) {
        result.emplace_back(k, v);
    };
    map.inorder(cb);
    vector<pair<int, string>> expected = {
        {3, "three"}, {2, "two"}, {1, "one"}
    };
    EXPECT_EQ(result, expected);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}