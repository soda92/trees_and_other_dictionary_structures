#include <fstream>
#include <random>
#include <vector>
#include <algorithm>

std::vector<int> generate_random(int n = 50) {
    auto range_max = 1000;
    if (n > range_max)
        n = range_max;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, range_max);

    auto results = std::vector<int>();
    auto exists = [&](int val) {
        return std::find(results.begin(), results.end(), val) != results.end();
    };
    for (auto i = 0; i < n; i++) {
        auto val = dist(gen);
        while (exists(val))
            val = dist(gen);
        results.push_back(val);
    }
    return results;
}

void write_file(std::vector<int> data) {
    auto out = std::ofstream("data.txt");
    for (auto number : data) {
        out << number << " ";
    }
}

int main() {
    auto data = generate_random();
    write_file(data);
}