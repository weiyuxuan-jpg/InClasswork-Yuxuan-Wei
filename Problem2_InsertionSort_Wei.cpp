#include <iostream>
#include <vector>

void printArray(const std::vector<int>& values) {
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (i != 0) std::cout << ' ';
        std::cout << values[i];
    }
    std::cout << '\n';
}

void insertionSort(std::vector<int>& values, unsigned long long& comparisons,
                   unsigned long long& shifts) {
    comparisons = shifts = 0;
    for (int i = 1; i < static_cast<int>(values.size()); ++i) {
        const int key = values[i];
        int j = i - 1;
        while (j >= 0) {
            ++comparisons;
            if (values[j] <= key) break;
            values[j + 1] = values[j];
            ++shifts;
            --j;
        }
        values[j + 1] = key;
        std::cout << "i = " << i << ", key = " << key << ": ";
        printArray(values);
    }
}

int main() {
    std::vector<int> values = {34, 7, 23, 32, 5, 62, 14, 19};
    unsigned long long comparisons = 0, shifts = 0;
    std::cout << "Problem 2: Insertion Sort\nOriginal: ";
    printArray(values);
    insertionSort(values, comparisons, shifts);
    std::cout << "Sorted: ";
    printArray(values);
    std::cout << "Comparisons: " << comparisons << "  Shifts: " << shifts << '\n';
}
