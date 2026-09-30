#include <iostream>
#include <vector>

void printRange(const std::vector<int>& values, int low, int high) {
    for (int i = low; i <= high; ++i) {
        if (i != low) std::cout << ' ';
        std::cout << values[i];
    }
    std::cout << '\n';
}

void mergeSort(std::vector<int>& values, std::vector<int>& buffer,
               int low, int high, unsigned long long& comparisons) {
    if (low >= high) return;
    const int mid = low + (high - low) / 2;
    std::cout << "split: ";
    printRange(values, low, high);
    mergeSort(values, buffer, low, mid, comparisons);
    mergeSort(values, buffer, mid + 1, high, comparisons);

    int left = low, right = mid + 1, out = low;
    while (left <= mid && right <= high) {
        ++comparisons;
        if (values[left] <= values[right]) buffer[out++] = values[left++];
        else buffer[out++] = values[right++];
    }
    while (left <= mid) buffer[out++] = values[left++];
    while (right <= high) buffer[out++] = values[right++];
    for (int i = low; i <= high; ++i) values[i] = buffer[i];
    std::cout << "merged: ";
    printRange(values, low, high);
}

int main() {
    std::vector<int> values = {34, 7, 23, 32, 5, 62, 14, 19};
    std::vector<int> buffer(values.size());
    unsigned long long comparisons = 0;
    std::cout << "Problem 5: Merge Sort\nOriginal: ";
    printRange(values, 0, static_cast<int>(values.size()) - 1);
    mergeSort(values, buffer, 0, static_cast<int>(values.size()) - 1, comparisons);
    std::cout << "Sorted: ";
    printRange(values, 0, static_cast<int>(values.size()) - 1);
    std::cout << "Comparisons while merging: " << comparisons << '\n';
}
