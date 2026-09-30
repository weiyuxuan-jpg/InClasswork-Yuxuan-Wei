#include <iostream>
#include <utility>
#include <vector>

void printRange(const std::vector<int>& values, int begin, int end) {
    for (int i = begin; i < end; ++i) {
        if (i != begin) std::cout << ' ';
        std::cout << values[i];
    }
    std::cout << '\n';
}

void siftDown(std::vector<int>& values, int root, int size,
              unsigned long long& swaps) {
    while (2 * root + 1 < size) {
        int child = 2 * root + 1;
        if (child + 1 < size && values[child + 1] > values[child]) ++child;
        if (values[root] >= values[child]) break;
        std::swap(values[root], values[child]);
        ++swaps;
        root = child;
    }
}

void heapSort(std::vector<int>& values, unsigned long long& swaps) {
    swaps = 0;
    const int n = static_cast<int>(values.size());
    for (int root = n / 2 - 1; root >= 0; --root)
        siftDown(values, root, n, swaps);
    std::cout << "Heap: ";
    printRange(values, 0, n);

    for (int end = n - 1; end >= 1; --end) {
        std::swap(values[0], values[end]);
        siftDown(values, 0, end, swaps);
        std::cout << "end = " << end << ": heap: ";
        printRange(values, 0, end);
        std::cout << "sorted: ";
        printRange(values, end, n);
    }
}

int main() {
    std::vector<int> values = {34, 7, 23, 32, 5, 62, 14, 19};
    unsigned long long swaps = 0;
    std::cout << "Problem 6: Heap Sort\nOriginal: ";
    printRange(values, 0, static_cast<int>(values.size()));
    heapSort(values, swaps);
    std::cout << "Sorted: ";
    printRange(values, 0, static_cast<int>(values.size()));
    std::cout << "Swaps in siftDown: " << swaps << '\n';
}
