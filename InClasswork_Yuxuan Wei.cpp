#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <utility>
#include <vector>

using namespace std;
using Counter = unsigned long long;
const vector<int> ORIGINAL = {34, 7, 23, 32, 5, 62, 14, 19};

void printArray(const int arr[], int n) {
    for (int i = 0; i < n; ++i) {
        if (i) cout << ' ';
        cout << arr[i];
    }
    cout << '\n';
}

void printArray(const vector<int>& arr) {
    printArray(arr.data(), static_cast<int>(arr.size()));
}

void printRange(const vector<int>& arr, int low, int high) {
    if (low > high) { cout << "(empty)\n"; return; }
    printArray(arr.data() + low, high - low + 1);
}

void heading(const string& title) {
    cout << "\n========== " << title << " ==========\n";
}

void bubbleSort(vector<int>& a, Counter& comparisons, Counter& swaps, bool show) {
    comparisons = swaps = 0;
    for (int pass = 0; pass + 1 < static_cast<int>(a.size()); ++pass) {
        bool changed = false;
        for (int j = 0; j + 1 < static_cast<int>(a.size()) - pass; ++j) {
            ++comparisons;
            if (a[j] > a[j + 1]) {
                swap(a[j], a[j + 1]); ++swaps; changed = true;
            }
        }
        if (show) { cout << "Pass " << pass + 1 << ": "; printArray(a); }
        if (!changed) break;
    }
}

void insertionSort(vector<int>& a, Counter& comparisons, Counter& shifts, bool show) {
    comparisons = shifts = 0;
    for (int i = 1; i < static_cast<int>(a.size()); ++i) {
        int key = a[i], j = i - 1;
        while (j >= 0) {
            ++comparisons;
            if (a[j] <= key) break;
            a[j + 1] = a[j]; ++shifts; --j;
        }
        a[j + 1] = key;
        if (show) { cout << "i = " << i << ", key = " << key << ": "; printArray(a); }
    }
}

void selectionSort(vector<int>& a, Counter& comparisons, Counter& swaps, bool show) {
    comparisons = swaps = 0;
    for (int i = 0; i + 1 < static_cast<int>(a.size()); ++i) {
        int minIndex = i;
        for (int j = i + 1; j < static_cast<int>(a.size()); ++j) {
            ++comparisons;
            if (a[j] < a[minIndex]) minIndex = j;
        }
        if (minIndex != i) { swap(a[i], a[minIndex]); ++swaps; }
        if (show) {
            cout << "Pass " << i + 1 << ": min = " << a[i]
                 << " at index " << minIndex << " -> ";
            printArray(a);
        }
    }
}

void quickSort(vector<int>& a, int low, int high, Counter& comparisons,
               int depth, bool show) {
    if (low >= high) return;
    if (show) {
        cout << string(depth * 2, ' ') << "pivot = " << a[high]
             << ", subarray: "; printRange(a, low, high);
    }

    auto partition = [&](int rangeLow, int rangeHigh) {
        int pivot = a[rangeHigh];
        int i = rangeLow;

        for (int j = rangeLow; j < rangeHigh; ++j) {
            ++comparisons;
            if (a[j] <= pivot) {
                if (i != j) {
                    swap(a[i], a[j]);
                    if (show) {
                        cout << string(depth * 2, ' ') << "swap i=" << i
                             << ", j=" << j << ": ";
                        printRange(a, rangeLow, rangeHigh);
                    }
                }
                ++i;
            }
        }

        if (i != rangeHigh) {
            swap(a[i], a[rangeHigh]);
            if (show) {
                cout << string(depth * 2, ' ') << "pivot " << pivot
                     << " final index = " << i << ": ";
                printRange(a, rangeLow, rangeHigh);
            }
        }
        return i;
    };

    int p = partition(low, high);
    quickSort(a, low, p - 1, comparisons, depth + 1, show);
    quickSort(a, p + 1, high, comparisons, depth + 1, show);
}

void mergeSort(vector<int>& a, vector<int>& buffer, int low, int high,
               Counter& comparisons, bool show,
               Counter* finalMergeComparisons = nullptr) {
    if (low >= high) return;
    int mid = low + (high - low) / 2;
    if (show) { cout << "split: "; printRange(a, low, high); }
    mergeSort(a, buffer, low, mid, comparisons, show);
    mergeSort(a, buffer, mid + 1, high, comparisons, show);

    auto MERGE = [&](int rangeLow, int middle, int rangeHigh) {
        int i = rangeLow;
        int j = middle + 1;
        int out = rangeLow;
        while (i <= middle && j <= rangeHigh) {
            ++comparisons;
            if (a[i] <= a[j]) buffer[out++] = a[i++];
            else buffer[out++] = a[j++];
        }
        while (i <= middle) buffer[out++] = a[i++];
        while (j <= rangeHigh) buffer[out++] = a[j++];
        for (int k = rangeLow; k <= rangeHigh; ++k) a[k] = buffer[k];
        if (show) { cout << "merged: "; printRange(a, rangeLow, rangeHigh); }
    };

    Counter beforeMerge = comparisons;
    MERGE(low, mid, high);
    if (finalMergeComparisons != nullptr &&
        low == 0 && high == static_cast<int>(a.size()) - 1) {
        *finalMergeComparisons = comparisons - beforeMerge;
    }
}

void heapSort(vector<int>& a, Counter& siftSwaps, bool show) {
    siftSwaps = 0;
    int n = static_cast<int>(a.size());

    auto siftDown = [&](int startRoot, int size) {
        int root = startRoot;
        while (true) {
            int largest = root;
            int left = 2 * root + 1;
            int right = left + 1;
            if (left < size && a[left] > a[largest]) largest = left;
            if (right < size && a[right] > a[largest]) largest = right;
            if (largest == root) return;
            swap(a[root], a[largest]);
            ++siftSwaps;
            root = largest;
        }
    };

    for (int root = n / 2 - 1; root >= 0; --root)
        siftDown(root, n);
    if (show) { cout << "Heap: "; printArray(a); }
    for (int end = n - 1; end >= 1; --end) {
        swap(a[0], a[end]);
        siftDown(0, end);
        if (show) {
            cout << "end = " << end << ": heap: "; printArray(a.data(), end);
            cout << "sorted: "; printArray(a.data() + end, n - end);
        }
    }
}

void run1() {
    heading("Problem 1: Bubble Sort");
    vector<int> a = ORIGINAL; Counter c, s;
    cout << "Original: "; printArray(a); bubbleSort(a, c, s, true);
    cout << "Sorted:   "; printArray(a);
    cout << "Comparisons: " << c << "  Swaps: " << s << '\n';
    vector<int> sorted = a; bubbleSort(sorted, c, s, true);
    cout << "Already sorted input: comparisons = " << c << ", swaps = " << s << '\n';
}

void run2() {
    heading("Problem 2: Insertion Sort");
    vector<int> a = ORIGINAL; Counter c, s;
    cout << "Original: "; printArray(a); insertionSort(a, c, s, true);
    cout << "Sorted:   "; printArray(a);
    cout << "Comparisons: " << c << "  Shifts: " << s << '\n';
    vector<int> best = a, worst = a; reverse(worst.begin(), worst.end());
    insertionSort(best, c, s, false); Counter bestShifts = s;
    insertionSort(worst, c, s, false); Counter worstShifts = s;
    cout << "Sorted input shifts (fewest): " << bestShifts << '\n';
    cout << "Reverse-sorted input shifts (most): " << worstShifts << '\n';
}

void run3() {
    heading("Problem 3: Selection Sort");
    vector<int> a = ORIGINAL; Counter c, s;
    cout << "Original: "; printArray(a); selectionSort(a, c, s, true);
    cout << "Sorted:   "; printArray(a);
    cout << "Comparisons: " << c << "  Swaps: " << s << '\n';
    cout << "Selection Sort performs at most one swap per pass, so it normally "
         << "uses fewer swaps than Bubble Sort on this input.\n";
}

void run4() {
    heading("Problem 4: Quick Sort (Lomuto)");
    vector<int> a = ORIGINAL; Counter c = 0;
    cout << "Original: "; printArray(a);
    quickSort(a, 0, static_cast<int>(a.size()) - 1, c, 0, true);
    cout << "Sorted:   "; printArray(a);
    cout << "Comparisons: " << c << '\n';
    vector<int> sorted = a; c = 0;
    quickSort(sorted, 0, static_cast<int>(sorted.size()) - 1, c, 0, false);
    cout << "Already sorted input comparisons: " << c << '\n';
    cout << "Last-element pivots produce unbalanced partitions on sorted data; "
         << "randomized or median-of-three pivots reduce this risk.\n";
}

void run5() {
    heading("Problem 5: Merge Sort");
    vector<int> a = ORIGINAL, buffer(a.size()); Counter c = 0;
    cout << "Original: "; printArray(a);
    Counter finalComparisons = 0;
    mergeSort(a, buffer, 0, static_cast<int>(a.size()) - 1, c, true,
              &finalComparisons);
    cout << "Sorted:   "; printArray(a);
    cout << "Comparisons: " << c
         << "  (final merge: " << finalComparisons << ")\n";

    vector<int> sorted = a, reversed = a; reverse(reversed.begin(), reversed.end());
    Counter sortedC = 0, reversedC = 0;
    buffer.assign(sorted.size(), 0);
    mergeSort(sorted, buffer, 0, static_cast<int>(sorted.size()) - 1, sortedC, false);
    buffer.assign(reversed.size(), 0);
    mergeSort(reversed, buffer, 0, static_cast<int>(reversed.size()) - 1, reversedC, false);
}

void run6() {
    heading("Problem 6: Heap Sort");
    vector<int> a = ORIGINAL; Counter s = 0;
    cout << "Original: "; printArray(a); heapSort(a, s, true);
    cout << "Sorted:   "; printArray(a);
    cout << "Swaps in siftDown: " << s << '\n';
    cout << "Heap Sort is not stable: swaps can change the relative order of equal "
         << "values.\n";
}

using Sorter = void (*)(vector<int>&, Counter&, bool);
void bubbleForComparison(vector<int>& a, Counter& c, bool show) {
    Counter swaps = 0;
    bubbleSort(a, c, swaps, show);
}
void insertionForComparison(vector<int>& a, Counter& c, bool show) {
    Counter shifts = 0;
    insertionSort(a, c, shifts, show);
}
void selectionForComparison(vector<int>& a, Counter& c, bool show) {
    Counter swaps = 0;
    selectionSort(a, c, swaps, show);
}
void quickForComparison(vector<int>& a, Counter& c, bool show) {
    quickSort(a, 0, static_cast<int>(a.size()) - 1, c, 0, show);
}
void mergeForComparison(vector<int>& a, Counter& c, bool show) {
    vector<int> buffer(a.size());
    mergeSort(a, buffer, 0, static_cast<int>(a.size()) - 1, c, show);
}

void showComparison(const string& name, Sorter sorter, const vector<int>& input) {
    vector<int> a = input; Counter c = 0; sorter(a, c, false);
    cout << left << setw(14) << name << right << setw(14) << c << "  ";
    printArray(a.data(), min(10, static_cast<int>(a.size())));
}

void run7() {
    heading("Problem 7: Comparing the Six Algorithms");
    vector<int> sorted = ORIGINAL, reversed = ORIGINAL;
    Counter setupComparisons = 0, setupSwaps = 0;
    selectionSort(sorted, setupComparisons, setupSwaps, false);
    reverse(reversed.begin(), reversed.end());
    const vector<pair<string, Sorter>> algorithms = {
        pair<string, Sorter>{"Bubble", bubbleForComparison},
        pair<string, Sorter>{"Insertion", insertionForComparison},
        pair<string, Sorter>{"Selection", selectionForComparison},
        pair<string, Sorter>{"Quick", static_cast<Sorter>(quickForComparison)},
        pair<string, Sorter>{"Merge", static_cast<Sorter>(mergeForComparison)},
        pair<string, Sorter>{"Heap", static_cast<Sorter>(heapSort)}
    };
    cout << "Comparison counts; each row also prints the first 10 sorted values.\n";
    for (const auto& item : algorithms) {
        cout << "\n" << item.first << "\n";
        cout << left << setw(14) << "Input" << right << setw(14)
             << "Comparisons" << "  First 10 values\n";
        showComparison("Original", item.second, ORIGINAL);
        showComparison("Sorted", item.second, sorted);
        showComparison("Reversed", item.second, reversed);
    }

    cout << "\nTiming random arrays (milliseconds)\n";
    mt19937 generator(7205); uniform_int_distribution<int> distribution(1, 100000);
    cout << left << setw(12) << "n";
    for (const auto& item : algorithms) cout << setw(14) << item.first;
    cout << '\n';
    for (int n : {1000, 5000, 10000}) {
        vector<int> data(n);
        for (int& value : data) value = distribution(generator);
        cout << left << setw(12) << n;
        for (const auto& item : algorithms) {
            vector<int> copy = data; Counter ignored = 0;
            auto start = chrono::steady_clock::now();
            item.second(copy, ignored, false);
            auto finish = chrono::steady_clock::now();
            double ms = chrono::duration<double, milli>(finish - start).count();
            cout << setw(14) << fixed << setprecision(3) << ms;
        }
        cout << '\n';
    }
    cout << "\nTheory: Bubble, Insertion, Selection are O(n^2); Merge and Heap are "
         << "O(n log n); Quick is O(n log n) average and O(n^2) worst case.\n";
    cout << "Merge or Heap is predictable for large random arrays; Insertion can "
         << "be effective for nearly sorted data.\n";
}

int main() {
    cout << "EECE 7205 In-Class Sorting Work\n"
         << "1. Bubble Sort\n2. Insertion Sort\n3. Selection Sort\n"
         << "4. Quick Sort\n5. Merge Sort\n6. Heap Sort\n"
         << "7. Compare all six algorithms\nChoose a problem (1-7): ";
    int choice;
    if (!(cin >> choice)) { cerr << "Invalid choice.\n"; return 1; }
    switch (choice) {
        case 1: run1(); break; case 2: run2(); break; case 3: run3(); break;
        case 4: run4(); break; case 5: run5(); break; case 6: run6(); break;
        case 7: run7(); break;
        default: cerr << "Please choose a number from 1 to 7.\n"; return 1;
    }
    return 0;
}
