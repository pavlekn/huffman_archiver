#ifndef ARCHIVER_HEAP_H
#define ARCHIVER_HEAP_H

#include <cstdint>
#include <vector>

template <typename T>
class Heap {
public:
    Heap () {}
    T Pop() { // min on the top
        T min = elements_[0];
        std::swap(elements_[0], elements_.back());
        elements_.pop_back();
        size_t ind = 0;
        while (ind * 2 + 1 < elements_.size()) {
            if (ind * 2 + 2 == elements_.size() || elements_[ind * 2 + 1] < elements_[ind * 2 + 2]) {
                if (elements_[ind] < elements_[ind * 2 + 1]) {
                    break;
                }
                std::swap(elements_[ind], elements_[2 * ind + 1]);
                ind = ind * 2 + 1;
            } else {
                if (elements_[ind] < elements_[ind * 2 + 2]) {
                    break;
                }
                std::swap(elements_[ind], elements_[2 * ind + 2]);
                ind = ind * 2 + 2;
            }
        }
        return min;
    }
    void Push(T x) {
        elements_.push_back(x);
        size_t ind = elements_.size() - 1;
        while (ind > 0 && x < elements_[(ind - 1) / 2]) {
            std::swap(elements_[ind], elements_[(ind - 1) / 2]);
            ind = (ind - 1) / 2;
        }
    }
    size_t Size() const {
        return elements_.size();
    }

private:
    std::vector<T> elements_;
};

#endif //ARCHIVER_HEAP_H
