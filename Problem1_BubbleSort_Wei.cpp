#include <iostream>
#include <utility>
#include <vector>

void printArray(const std::vector<int>& values) {
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (i != 0) std::cout << ' ';
        std::cout << values[i];
    }
    std::cout << '\n';
}

void bubbleSort(std::vector<int>& values, unsigned long long& comparisons,
                unsigned long long& swaps) {
    comparisons = swaps = 0;
    for (int pass = 0; pass + 1 < static_cast<int>(values.size()); ++pass) {
        bool swapped = false;
        for (int i = 0; i + 1 < static_cast<int>(values.size()) - pass; ++i) {
            ++comparisons;
            if (values[i] > values[i + 1]) {
                std::swap(values[i], values[i + 1]);
                ++swaps;
                swapped = true;
            }
        }
        std::cout << "Pass " << pass + 1 << ": ";
        printArray(values);
        if (!swapped) break;
    }
}

int main() {
    std::vector<int> values = {34, 7, 23, 32, 5, 62, 14, 19};
    unsigned long long comparisons = 0, swaps = 0;
    std::cout << "Problem 1: Bubble Sort\nOriginal: ";
    printArray(values);
    bubbleSort(values, comparisons, swaps);
    std::cout << "Sorted: ";
    printArray(values);
    std::cout << "Comparisons: " << comparisons << "  Swaps: " << swaps << '\n';
}
