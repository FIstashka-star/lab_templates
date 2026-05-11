#pragma once

#include <algorithm>
#include <cstddef>

template <typename T>
class Array {
public:
    // ======================== Constructors ========================
    Array() : data_(nullptr), size_(0) {}

    explicit Array(size_t size) : data_(new T[size]()), size_(size) {}

    Array(size_t size, const T& value) : data_(new T[size]), size_(size) {
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = value;
        }
    }

    // =================== Copy and Move ===========================
    Array(const Array& other) : data_(new T[other.size_]), size_(other.size_) {
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }

    Array(Array&& other) noexcept
        : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
    }

    Array& operator=(const Array& other) {
        if (this != &other) {
            Array tmp(other);
            swap(tmp);
        }
        return *this;
    }

    Array& operator=(Array&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    // =================== Destructor ==============================
    ~Array() {
        delete[] data_;
    }

    // =================== Element access ==========================
    T& operator[](size_t index) {
        return data_[index];
    }

    const T& operator[](size_t index) const {
        return data_[index];
    }

    T& front() {
        return data_[0];
    }

    const T& front() const {
        return data_[0];
    }

    T& back() {
        return data_[size_ - 1];
    }

    const T& back() const {
        return data_[size_ - 1];
    }

    T* data() {
        return data_;
    }

    const T* data() const {
        return data_;
    }

    // =================== Capacity ================================
    size_t size() const {
        return size_;
    }

    bool empty() const {
        return size_ == 0;
    }

    // =================== Operations ==============================
    void fill(const T& value) {
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = value;
        }
    }

    void swap(Array& other) noexcept {
        using std::swap;
        swap(data_, other.data_);
        swap(size_, other.size_);
    }

    // =================== Comparison ==============================
    bool operator==(const Array& rhs) const {
        if (size_ != rhs.size_) {
            return false;
        }
        for (size_t i = 0; i < size_; ++i) {
            if (data_[i] != rhs.data_[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const Array& rhs) const {
        return !(*this == rhs);
    }

private:
    T* data_;
    size_t size_;
};