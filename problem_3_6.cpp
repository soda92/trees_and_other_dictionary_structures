#include <algorithm>
#include <fstream>
#include <functional>
#include <ios>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

template <typename T> struct bintree {
    bintree *left{nullptr};
    bintree *right{nullptr};
    T data;

    bintree<T>() {}
    bintree<T>(T data) { this->data = data; }
};

template <typename T> void insert_tree(bintree<T> *&tree, T data);

template <typename T> bintree<T> *create_tree(std::vector<T> data) {
    if (data.size() == 0)
        return nullptr;
    auto head = new bintree<T>();
    head->data = data.at(0);

    for (auto i = 1; i < data.size(); i++) {
        if (data.at(i) < head->data) {
            insert_tree(head->left, data.at(i));
        } else if (data.at(i) > head->data) {
            insert_tree(head->right, data.at(i));
        }
    }
    return head;
}

template <typename T> void insert_tree(bintree<T> *&tree, T data) {
    if (tree == nullptr)
        tree = new bintree<T>(data);
    else {
        if (data < tree->data) {
            insert_tree(tree->left, data);
        } else if (data > tree->data) {
            insert_tree(tree->right, data);
        }
    }
}

template <typename T>
void in_order_traverse(bintree<T> *tree, std::function<void(T)> op) {
    if (tree == nullptr)
        return;
    else {
        in_order_traverse(tree->left, op);
        op(tree->data);
        in_order_traverse(tree->right, op);
    }
}

template <typename T> void in_order_print(bintree<T> *tree) {
    auto op = [](T data) { std::cout << data << ' '; };
    in_order_traverse(tree, std::function<void(T)>(op));
    std::cout << '\n';
}

template <typename T> std::vector<T> in_order_data(bintree<T> *tree) {
    auto data_ = std::vector<T>();
    auto op = [&](T data) { data_.push_back(data); };
    in_order_traverse(tree, std::function<void(T)>(op));
    return data_;
}

std::vector<int> read_data() {
    auto in = std::ifstream("data.txt", std::ios_base::in);
    auto line = std::string();
    auto data = std::vector<int>();
    while(std::getline(in, line)) {
        auto sstream = std::stringstream(line);
        auto number = int{0};
        while(sstream >> number) {
            data.push_back(number);
        }
    }
    return data;
}

void write_file(std::vector<int> data) {
    auto out = std::ofstream("data-sorted.txt");
    for (auto number : data) {
        out << number << " ";
    }
}

int main() {
    auto numbers = read_data();
    auto tree = create_tree(numbers);
    in_order_print(tree);
    auto data = in_order_data(tree);
    if (std::is_sorted(data.begin(), data.end())) {
        std::cout << "sorted\n";
    }
    write_file(data);
}
