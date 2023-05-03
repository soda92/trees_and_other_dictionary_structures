#include <algorithm>
#include <cstddef>
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

    bintree *successor{nullptr};
    bintree *predecessor{nullptr};

    bintree<T>() {}
    bintree<T>(T data) { this->data = data; }
};

enum class Direction { Left, Right };

template <typename T> struct insertion_path {
    std::vector<Direction> directions;
    std::vector<bintree<T> *> pointers;
};

template <typename T>
void insert_tree(bintree<T> *&tree, T data, insertion_path<T> &path);

template <typename T> bintree<T> *create_tree(std::vector<T> data) {
    if (data.size() == 0)
        return nullptr;
    auto head = new bintree<T>(data.at(0));

    for (auto i = 1; i < data.size(); i++) {
        auto path = insertion_path<T>();
        insert_tree(head, data.at(i), path);
    }
    return head;
}

template <typename T> T &last(std::vector<T> &data) {
    return data.at(data.size() - 1);
}

template <typename T, Direction D1, Direction D2>
bintree<T> *find_path(insertion_path<T> path) {
    if (path.pointers.size() == 0)
        return nullptr;
    if (last(path.directions) == D1) {
        return last(path.pointers);
    } else {
        while (last(path.directions) == D2) {
            path.directions.pop_back();
            path.pointers.pop_back();
            if (path.directions.size() == 0)
                return nullptr;
        }
        return last(path.pointers);
    }
}

template <typename T> bintree<T> *get_successor(insertion_path<T> path) {
    return find_path<T, Direction::Left, Direction::Right>(path);
}

template <typename T> bintree<T> *get_predecessor(insertion_path<T> path) {
    return find_path<T, Direction::Right, Direction::Left>(path);
}

template <typename T>
void insert_tree(bintree<T> *&tree, T data, insertion_path<T> &path) {
    if (tree == nullptr) {
        tree = new bintree<T>(data);
        auto successor = get_successor(path);
        if (successor) {
            auto previous_predecessor = successor->predecessor;
            successor->predecessor = tree;
            tree->successor = successor;
            if (previous_predecessor) {
                previous_predecessor->successor = tree;
                tree->predecessor = previous_predecessor;
            }
        } else {
            auto predecessor = get_predecessor(path);
            if (predecessor) {
                auto previous_successor = predecessor->successor;
                predecessor->successor = tree;
                tree->predecessor = predecessor;
                if (previous_successor) {
                    previous_successor->predecessor = tree;
                    tree->successor = previous_successor;
                }
            }
        }
    } else {
        path.pointers.push_back(tree);
        if (data < tree->data) {
            path.directions.push_back(Direction::Left);
            insert_tree(tree->left, data, path);
        } else if (data > tree->data) {
            path.directions.push_back(Direction::Right);
            insert_tree(tree->right, data, path);
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
    std::function<void(T)> op = [](T data) { std::cout << data << ' '; };
    in_order_traverse(tree, op);
    std::cout << '\n';
}

template <typename T> std::vector<T> in_order_data(bintree<T> *tree) {
    auto data_ = std::vector<T>();
    std::function<void(T)> op = [&](T data) { data_.push_back(data); };
    in_order_traverse(tree, op);
    return data_;
}

template <typename T> std::vector<T> traverse_successor(bintree<T> *tree) {
    auto data = std::vector<T>();
    if (tree != nullptr) {
        while (tree->predecessor != nullptr) {
            tree = tree->predecessor;
        }
    }
    while (tree != nullptr) {
        data.push_back(tree->data);
        tree = tree->successor;
    }
    return data;
}

std::vector<int> read_data() {
    auto in = std::ifstream("data.txt", std::ios_base::in);
    auto line = std::string();
    auto data = std::vector<int>();
    while (std::getline(in, line)) {
        auto sstream = std::stringstream(line);
        auto number = int{0};
        while (sstream >> number) {
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
    for (auto i : numbers) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
    auto tree = create_tree(numbers);
    in_order_print(tree);
    auto data = in_order_data(tree);
    if (std::is_sorted(data.begin(), data.end())) {
        std::cout << "sorted\n";
    }
    write_file(data);
    auto data_successor_traverse = traverse_successor(tree);
    for (auto i : data_successor_traverse) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
    if (data == data_successor_traverse) {
        std::cout << "equal\n";
    } else {
        std::cout << "not equal\n";
    }
}
