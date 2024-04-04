#include <iostream>
#include <vector>
#include <memory> 

template<typename T>
const auto DEFAULT_CMP = [](const T& a, const T& b) {return a < b;};
template<typename T>
const auto FALSE_CMP = [](const T& a, const T& b) {return a >= b;};

template<std::equality_comparable T = int>
class BubbleSort {
public:
    template<typename CMP = decltype(DEFAULT_CMP<T>)>
    void sort(std::vector<T>& data, 
    CMP cmp = DEFAULT_CMP<T>
    ) const {
        int n = data.size();
        for (int i = 0; i < n - 1; ++i) {
            for (int j = 0; j < n - i - 1; ++j) {
                if (cmp(data[j + 1], data[j])) {
                    std::swap(data[j], data[j + 1]);
                }
            }
        }
    }
};

template<std::equality_comparable T = int>
class QuickSort {
public:
    template<typename CMP = decltype(DEFAULT_CMP<T>)>
    void sort(std::vector<T>& data,
    CMP cmp = DEFAULT_CMP<T>) const {
        quickSort(data, 0, data.size() - 1, cmp);
    }

private:
    template<typename CMP>
    void quickSort(std::vector<T>& data, int low, int high, CMP cmp) const {
        if (low < high) {
            int pivot = partition(data, low, high, cmp);
            quickSort(data, low, pivot - 1, cmp);
            quickSort(data, pivot + 1, high, cmp);
        }
    }

    template<typename CMP>
    int partition(std::vector<T>& data, int low, int high, CMP cmp) const {
        T pivot = data[high];
        int i = low - 1;
        for (int j = low; j <= high - 1; ++j) {
            if (cmp(data[j], pivot)) {
                ++i;
                std::swap(data[i], data[j]);
            }
        }
        std::swap(data[i + 1], data[high]);
        return i + 1;
    }
};



// Контекст
template<std::default_initializable Strat, std::equality_comparable T = int>
requires requires(const Strat a, std::vector<T>& data) {
  a.sort(data);
}
class ContextSort {
private:
    std::shared_ptr<Strat> strategy;

public:
    ContextSort() : strategy(std::make_shared<Strat>()) {}

    template<typename CMP = decltype(DEFAULT_CMP<T>)>
    void performSort(std::vector<T>& data, CMP cmp = DEFAULT_CMP<T>) {
        strategy->sort(data, cmp);
    }
};

int main() {
    std::vector<int> data = {5, 2, 7, 1, 9, 3};

    // Использование стратегии сортировки пузырьком
    ContextSort<BubbleSort<>> context1;
    context1.performSort(data, FALSE_CMP<int>);
    std::cout << "Bubble Sort: ";
    for (int num : data) {
        std::cout << num << " ";
    }
    std::cout << '\n';

    // Использование стратегии быстрой сортировки
    ContextSort<QuickSort<>> context2;
    context2.performSort(data);
    std::cout << "Quick Sort: ";
    for (int num : data) {
        std::cout << num << " ";
    }
    std::cout << '\n';

    return 0;
}
