#ifndef BORDER
#define BORDER
#include <iostream>
#include <exception>
#include <algorithm>
#include <functional>
#include <string>
#include <stack>
using namespace std;

template<typename T, typename T1>
using callback = std::function<void(const T&, const T1&)>;

template<typename T, typename T1>
void printNode(const T& key, const T1& val) { cout << "(" << key << ", " << val << ")" << endl; }

template<typename T>
class Comparator {
public:
    virtual ~Comparator() = default;
    virtual bool compare(const T& a, const T& b) const = 0;
};

template<typename T>
class DefaultComparator : public Comparator<T> {
public:
    bool compare(const T& a, const T& b) const override final { return std::less<T>()(a, b); }
};

template<typename T>
class GreaterComparator : public Comparator<T> {
public:
    bool compare(const T& a, const T& b) const override final { return std::greater<T>()(a, b); }
};

template<typename Key, typename Value, template<typename> typename Comparator = DefaultComparator>
class BST {
private:
    class Node {
    public:
        Key key;
        Value val;
        Node* left, * right;
        Node() : key(Key{}), val(Value{}), left(nullptr), right(nullptr) {}
        Node(const Key& key, const Value& val) : key(key), val(val), left(nullptr), right(nullptr) {}
        ~Node() = default;
        Key get_key() const noexcept { return key; }
        Value get_val() const noexcept { return val; }
    };

private:
    Comparator<Key>* comp;
    Node* root;
    size_t sz;

public:
    BST() : root(nullptr), sz(0) { comp = new Comparator<Key>; }
    BST(Node*& root) : sz(1), root(root) { comp = new Comparator<Key>; }
    BST(const Node*& root) : sz(1), root(root) { comp = new Comparator<Key>; }
    void delete_all_nodes(Node*& cur) {
        if (!cur) {
            return;
        }
        delete_all_nodes(cur->left);
        delete_all_nodes(cur->right);
        delete cur;
        cur = nullptr;
    }
    void clear() {
        sz = 0;
        delete_all_nodes(root);
    }
    void insert(const Key& k, const Value& v) {
        if (!root) {
            sz++;
            root = new Node(k, v);
            return;
        }
        Node* cur = root;
        while (true) {
            if (comp->compare(k, cur->key)) {
                if (!cur->left) {
                    sz++;
                    cur->left = new Node(k, v);
                    return;
                }
                cur = cur->left;
            }
            else if (comp->compare(cur->key, k)) {
                if (!cur->right) {
                    sz++;
                    cur->right = new Node(k, v);
                    return;
                }
                cur = cur->right;
            }
            else {
                cur->val = v;
                return;
            }
        }
    }
    Value* find(const Key& k) const {
        Node* cur = root;
        while (cur) {
            if (comp->compare(k, cur->key)) {
                cur = cur->left;
            }
            else if (comp->compare(cur->key, k)) {
                cur = cur->right;
            }
            else {
                return &(cur->val);
            }
        }
        return nullptr;
    }
    void remove(const Key& k) {
        Node** parent_ptr = &root;
        Node* cur = root;
        while (cur) {
            if (comp->compare(k, cur->key)) {
                parent_ptr = &(cur->left);
                cur = cur->left;
            }
            else if (comp->compare(cur->key, k)) {
                parent_ptr = &(cur->right);
                cur = cur->right;
            }
            else {
                break;
            }
        }
        if (!cur) { return; }
        if (!cur->left || !cur->right) {
            Node* child = cur->left ? cur->left : cur->right;
            *parent_ptr = child;
            delete cur;
        }
        else {
            Node** min_parent_ptr = &(cur->right);
            Node* min_node = cur->right;
            while (min_node->left) {
                min_parent_ptr = &(min_node->left);
                min_node = min_node->left;
            }
            *min_parent_ptr = min_node->right;
            min_node->left = cur->left;
            min_node->right = cur->right;
            *parent_ptr = min_node;
            delete cur;
        }
        sz--;
    }
    /*void print_node(const Node* cur, const size_t offset) const{
        if (!cur){
            return;
        }
        for (size_t i = 0; i < offset; i++){
            cout << ' ';
        }
        cout <<"(" << cur->key << ", " << cur->val << ")\n";
        print_node(cur->left, offset + 1);
        print_node(cur->right, offset + 1);
    }
    void print(size_t offset = 0) const { cout << "Size: " << sz << endl; print_node(root, offset); }*/
    size_t size() const { return sz; }
    bool contains(const Key& k) const { return this->find(k); }
    ~BST() { delete comp; this->clear(); }
    void inorder(Node* node, callback<Key, Value> func) {
        if (!node) { return; }
        inorder(node->left, func);
        func(node->key, node->val);
        inorder(node->right, func);
    }
    void inorder(callback<Key, Value> func) { this->inorder(root, func); }
    void postorder(Node* node, callback<Key, Value> func) {
        if (!node) { return; }
        postorder(node->left, func);
        postorder(node->right, func);
        func(node->key, node->val);
    }
    void postorder(callback<Key, Value> func) { this->postorder(root, func); }
    void prefix(Node* node, callback<Key, Value> func) {
        if (!node) { return; }
        func(node->key, node->val);
        prefix(node->left, func);
        prefix(node->right, func);
    }
    void prefix(callback<Key, Value> func) { this->prefix(root, func); }
    BST(BST&& btree) noexcept : comp(btree.comp), root(btree.root), sz(btree.sz) {
        btree.root = nullptr;
        btree.sz = 0;
        btree.comp = nullptr;
    }
    Node* copy(const Node* node) {
        if (!node) { return nullptr; }
        Node* new_node = new Node(node->key, node->val);
        new_node->left = copy(node->left);
        new_node->right = copy(node->right);
        return new_node;
    }
    BST(const BST& btree) : comp(new Comparator<Key>), root(nullptr), sz(0) {
        if (btree.root) { this->root = copy(btree.root); sz = btree.size(); }
    }
    BST& operator=(const BST& btree) {
        this->clear();
        root = copy(btree.root);
        sz = btree.size();
        return *this;
    }

public:
    template <class IterType>
    class Iterator {
    protected:
        friend class BST;
        friend class Node;
        IterType* ptr = nullptr;
        stack<IterType*> Stack;

    public:
        Iterator() = default;
        Iterator(IterType* ptr) : ptr(ptr), Stack() {}
        Iterator(IterType* ptr, const stack<IterType*>& s) : ptr(ptr), Stack(s) {}
        Iterator& operator++() {
            if (!ptr) { return *this; }
            if (ptr->right) {
                ptr = ptr->right;
                while (ptr->left) {
                    Stack.push(ptr);
                    ptr = ptr->left;
                }
            }
            else {
                if (Stack.empty()) { ptr = nullptr; }
                else {
                    ptr = Stack.top();
                    Stack.pop();
                }
            }
            return *this;
        }
        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        bool operator==(const Iterator& other) const { return this->ptr == other.ptr; }
        bool operator!=(const Iterator& other) const { return !(*this == other); }
        IterType& operator*() {
            if (!ptr) { throw std::out_of_range("Dereference out of range iterator"); }
            return *this->ptr;
        }
        const IterType& operator*() const {
            if (!ptr) { throw std::out_of_range("Dereference out of range iterator"); }
            return *this->ptr;
        }
    };

    using iterator = Iterator<Node>;
    using const_iterator = Iterator<const Node>;
    iterator end() noexcept { return iterator(nullptr); }
    iterator begin() noexcept {
        Node* cur = root;
        if (!cur) { return iterator(nullptr); }
        stack<Node*> s;
        while (cur->left) {
            s.push(cur);
            cur = cur->left;
        }
        return iterator(cur, s);
    }
    const_iterator cbegin() const noexcept {
        const Node* cur = root;
        stack<const Node*> s;
        if (!cur) { return const_iterator(nullptr); }
        while (cur->left) {
            s.push(cur);
            cur = cur->left;
        }
        return const_iterator(cur, s);
    }
    const_iterator cend() const noexcept { return const_iterator(nullptr); }
};

template <typename Key, typename Value, template<typename, typename, template<typename> typename> class TreeType = BST, template<typename> typename Comparator = DefaultComparator>
class MyMap {
private:
    TreeType<Key, Value, Comparator> tree_;

public:
    MyMap() = default;
    MyMap(const MyMap& map) : tree_(map.tree_) {}
    MyMap(MyMap&& map) noexcept : tree_(std::move(map.tree_)) {}
    MyMap& operator=(const MyMap& map) {
        if (this != &map) { tree_ = map.tree_; }
        return *this;
    }
    ~MyMap() = default;

    void insert(const Key& key, const Value& value) { tree_.insert(key, value); }
    Value* find(const Key& key) const { return tree_.find(key); }
    void remove(const Key& key) { tree_.remove(key); }
    bool contains(const Key& key) const { return tree_.contains(key); }
    void clear() { tree_.clear(); }
    size_t size() const { return tree_.size(); }

    typename TreeType<Key, Value, Comparator>::iterator begin() { return tree_.begin(); }
    typename TreeType<Key, Value, Comparator>::iterator end() { return tree_.end(); }
    typename TreeType<Key, Value, Comparator>::const_iterator cbegin() const { return tree_.cbegin(); }
    typename TreeType<Key, Value, Comparator>::const_iterator cend() const { return tree_.cend(); }

    void inorder(callback<Key, Value> func) { tree_.inorder(func); }
    void postorder(callback<Key, Value> func) { tree_.postorder(func); }
    void prefix(callback<Key, Value> func) { tree_.prefix(func); }
};
#endif