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

int partition(std::vector<int>& values, int low, int high,
              unsigned long long& comparisons) {
    const int pivot = values[high];
    int boundary = low;
    for (int current = low; current < high; ++current) {
        ++comparisons;
        if (values[current] <= pivot) {
            std::swap(values[boundary], values[current]);
            ++boundary;
        }
    }
    std::swap(values[boundary], values[high]);
    std::cout << "Pivot " << pivot << " final index = " << boundary << ": ";
    printArray(values);
    return boundary;
}

void quickSort(std::vector<int>& values, int low, int high,
               unsigned long long& comparisons) {
    if (low >= high) return;
    const int pivotIndex = partition(values, low, high, comparisons);
    quickSort(values, low, pivotIndex - 1, comparisons);
    quickSort(values, pivotIndex + 1, high, comparisons);
}

int main() {
    std::vector<int> values = {34, 7, 23, 32, 5, 62, 14, 19};
    unsigned long long comparisons = 0;
    std::cout << "Problem 4: Quick Sort (Lomuto)\nOriginal: ";
    printArray(values);
    quickSort(values, 0, static_cast<int>(values.size()) - 1, comparisons);
    std::cout << "Sorted: ";
    printArray(values);
    std::cout << "Comparisons: " << comparisons << '\n';
}
