#pragma once

#include <new>
#include <utility>

template <typename T>
class Optional {
public:
    // ======================== Constructors ========================
    Optional() : has_value_(false) {}

    Optional(const T& value) : has_value_(true) {
        new (storage_) T(value);
    }

    Optional(T&& value) : has_value_(true) {
        new (storage_) T(std::move(value));
    }

    // =================== Copy and Move ===========================
    Optional(const Optional& other) : has_value_(other.has_value_) {
        if (has_value_) {
            new (storage_) T(other.value());
        }
    }

    Optional(Optional&& other) noexcept : has_value_(other.has_value_) {
        if (has_value_) {
            new (storage_) T(std::move(other.value()));
            other.reset();
        }
    }

    Optional& operator=(const Optional& other) {
        if (this != &other) {
            if (other.has_value_) {
                if (has_value_) {
                    value() = other.value();
                } else {
                    new (storage_) T(other.value());
                    has_value_ = true;
                }
            } else {
                reset();
            }
        }
        return *this;
    }

    Optional& operator=(Optional&& other) noexcept {
        if (this != &other) {
            if (other.has_value_) {
                if (has_value_) {
                    value() = std::move(other.value());
                } else {
                    new (storage_) T(std::move(other.value()));
                    has_value_ = true;
                }
                other.reset();
            } else {
                reset();
            }
        }
        return *this;
    }

    Optional& operator=(const T& value) {
        if (has_value_) {
            this->value() = value;
        } else {
            new (storage_) T(value);
            has_value_ = true;
        }
        return *this;
    }

    Optional& operator=(T&& value) {
        if (has_value_) {
            this->value() = std::move(value);
        } else {
            new (storage_) T(std::move(value));
            has_value_ = true;
        }
        return *this;
    }

    // =================== Destructor ==============================
    ~Optional() {
        reset();
    }

    // =================== Observers ===============================
    bool has_value() const {
        return has_value_;
    }

    explicit operator bool() const {
        return has_value_;
    }

    T& value() {
        return *reinterpret_cast<T*>(storage_);
    }

    const T& value() const {
        return *reinterpret_cast<const T*>(storage_);
    }

    T value_or(const T& default_value) const {
        return has_value_ ? value() : default_value;
    }

    T& operator*() {
        return value();
    }

    const T& operator*() const {
        return value();
    }

    T* operator->() {
        return &value();
    }

    const T* operator->() const {
        return &value();
    }

    // =================== Modifiers ===============================
    template <typename... Args>
    T& emplace(Args&&... args) {
        reset();
        new (storage_) T(std::forward<Args>(args)...);
        has_value_ = true;
        return value();
    }

    void reset() {
        if (has_value_) {
            value().~T();
            has_value_ = false;
        }
    }

    void swap(Optional& other) noexcept {
        using std::swap;
        if (has_value_ && other.has_value_) {
            swap(value(), other.value());
        } else if (has_value_) {
            new (other.storage_) T(std::move(value()));
            reset();
            other.has_value_ = true;
        } else if (other.has_value_) {
            new (storage_) T(std::move(other.value()));
            other.reset();
            has_value_ = true;
        }
    }

    // =================== Comparison ==============================
    bool operator==(const Optional& rhs) const {
        if (has_value_ != rhs.has_value_) return false;
        if (!has_value_) return true;
        return value() == rhs.value();
    }

    bool operator!=(const Optional& rhs) const {
        return !(*this == rhs);
    }

private:
    alignas(T) unsigned char storage_[sizeof(T)];
    bool has_value_;
};