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

void selectionSort(std::vector<int>& values, unsigned long long& comparisons,
                   unsigned long long& swaps) {
    comparisons = swaps = 0;
    for (int i = 0; i + 1 < static_cast<int>(values.size()); ++i) {
        int minIndex = i;
        for (int j = i + 1; j < static_cast<int>(values.size()); ++j) {
            ++comparisons;
            if (values[j] < values[minIndex]) minIndex = j;
        }
        if (minIndex != i) {
            std::swap(values[i], values[minIndex]);
            ++swaps;
        }
        std::cout << "Pass " << i + 1 << ": min = " << values[i]
                  << " at index " << minIndex << " -> ";
        printArray(values);
    }
}

int main() {
    std::vector<int> values = {34, 7, 23, 32, 5, 62, 14, 19};
    unsigned long long comparisons = 0, swaps = 0;
    std::cout << "Problem 3: Selection Sort\nOriginal: ";
    printArray(values);
    selectionSort(values, comparisons, swaps);
    std::cout << "Sorted: ";
    printArray(values);
    std::cout << "Comparisons: " << comparisons << "  Swaps: " << swaps << '\n';
}
