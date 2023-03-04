#include <iostream>
#include <map>
#include <chrono>
#include <cmath>
 

int main() {
    std::map<int, int> testmap {};
    for (int i {}; i < 8; i++) {
        testmap.clear();
        auto start = std::chrono::high_resolution_clock::now();
        int n = (int) std::pow(10, i);
        for (int j {}; j < n; j++)
            testmap[j] = j;
        auto finish = std::chrono::high_resolution_clock::now();
        std::cout << i << " " << 2 * sizeof(int) * n / (1024 * 1024) << " " << 
            std::chrono::duration_cast<std::chrono::nanoseconds> (finish - start).count() << " ";
    }
    return 0;
}