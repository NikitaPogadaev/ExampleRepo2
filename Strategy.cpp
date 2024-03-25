#include <iostream>
#include <vector>
#include <memory> // Для std::shared_ptr

// Интерфейс стратегии
class SortStrategy {
public:
    virtual void sort(std::vector<int>& data) const = 0;
    virtual ~SortStrategy() = default;
};

// Конкретная стратегия: сортировка пузырьком
class BubbleSort : public SortStrategy {
public:
    void sort(std::vector<int>& data) const override {
        int n = data.size();
        for (int i = 0; i < n - 1; ++i) {
            for (int j = 0; j < n - i - 1; ++j) {
                if (data[j] > data[j + 1]) {
                    std::swap(data[j], data[j + 1]);
                }
            }
        }
    }
};

// Конкретная стратегия: быстрая сортировка
class QuickSort : public SortStrategy {
public:
    void sort(std::vector<int>& data) const override {
        quickSort(data, 0, data.size() - 1);
    }

private:
    void quickSort(std::vector<int>& data, int low, int high) const {
        if (low < high) {
            int pivot = partition(data, low, high);
            quickSort(data, low, pivot - 1);
            quickSort(data, pivot + 1, high);
        }
    }

    int partition(std::vector<int>& data, int low, int high) const {
        int pivot = data[high];
        int i = low - 1;
        for (int j = low; j <= high - 1; ++j) {
            if (data[j] < pivot) {
                ++i;
                std::swap(data[i], data[j]);
            }
        }
        std::swap(data[i + 1], data[high]);
        return i + 1;
    }
};

// Контекст
class Context {
private:
    std::shared_ptr<SortStrategy> strategy;

public:
    Context(const std::shared_ptr<SortStrategy>& strategy) : strategy(strategy) {}

    void setStrategy(const std::shared_ptr<SortStrategy>& strategy) {
        this->strategy = strategy;
    }

    void performSort(std::vector<int>& data) {
        strategy->sort(data);
    }
};

int main() {
    std::vector<int> data = {5, 2, 7, 1, 9, 3};

    // Использование стратегии сортировки пузырьком
    auto bubbleSort = std::make_shared<BubbleSort>();
    Context context(bubbleSort);
    context.performSort(data);
    std::cout << "Bubble Sort: ";
    for (int num : data) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // Использование стратегии быстрой сортировки
    auto quickSort = std::make_shared<QuickSort>();
    context.setStrategy(quickSort);
    context.performSort(data);
    std::cout << "Quick Sort: ";
    for (int num : data) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}
