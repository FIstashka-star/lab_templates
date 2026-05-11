#pragma once

#include <cstddef>
#include <utility>
#include <memory>

// =================== Основной шаблон для одиночных объектов =================
template <typename T, typename Deleter = std::default_delete<T>>
class UniquePtr : private Deleter {  // EBO: пустой Deleter не увеличивает размер
public:
    // ======================== Constructors ========================
    UniquePtr() : ptr_(nullptr) {}

    explicit UniquePtr(T* ptr) : Deleter(), ptr_(ptr) {}

    UniquePtr(T* ptr, const Deleter& deleter) : Deleter(deleter), ptr_(ptr) {}

    UniquePtr(T* ptr, Deleter&& deleter) : Deleter(std::move(deleter)), ptr_(ptr) {}

    // =================== No copy ==================================
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // =================== Move =====================================
    UniquePtr(UniquePtr&& other) noexcept
        : Deleter(std::move(other.get_deleter())), ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            reset();
            ptr_ = other.ptr_;
            get_deleter() = std::move(other.get_deleter());
            other.ptr_ = nullptr;
        }
        return *this;
    }

    // =================== Destructor ===============================
    ~UniquePtr() {
        reset();
    }

    // =================== Observers ================================
    T* get() const {
        return ptr_;
    }

    explicit operator bool() const {
        return ptr_ != nullptr;
    }

    T& operator*() const {
        return *ptr_;
    }

    T* operator->() const {
        return ptr_;
    }

    Deleter& get_deleter() {
        return *this;
    }

    const Deleter& get_deleter() const {
        return *this;
    }

    // =================== Modifiers ================================
    T* release() {
        T* released_ptr = ptr_;
        ptr_ = nullptr;
        return released_ptr;
    }

    void reset(T* ptr = nullptr) {
        if (ptr_ != ptr) {
            if (ptr_) {
                get_deleter()(ptr_);
            }
            ptr_ = ptr;
        }
    }

    void swap(UniquePtr& other) noexcept {
        using std::swap;
        swap(ptr_, other.ptr_);
        swap(get_deleter(), other.get_deleter());
    }

private:
    T* ptr_;
};

// =================== Специализация для массивов =============================
template <typename T, typename Deleter>
class UniquePtr<T[], Deleter> : private Deleter {
public:
    // ======================== Constructors ========================
    UniquePtr() : ptr_(nullptr) {}

    explicit UniquePtr(T* ptr) : Deleter(), ptr_(ptr) {}

    UniquePtr(T* ptr, const Deleter& deleter) : Deleter(deleter), ptr_(ptr) {}

    UniquePtr(T* ptr, Deleter&& deleter) : Deleter(std::move(deleter)), ptr_(ptr) {}

    // =================== No copy ==================================
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // =================== Move =====================================
    UniquePtr(UniquePtr&& other) noexcept
        : Deleter(std::move(other.get_deleter())), ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            reset();
            ptr_ = other.ptr_;
            get_deleter() = std::move(other.get_deleter());
            other.ptr_ = nullptr;
        }
        return *this;
    }

    // =================== Destructor ===============================
    ~UniquePtr() {
        reset();
    }

    // =================== Observers ================================
    T* get() const {
        return ptr_;
    }

    explicit operator bool() const {
        return ptr_ != nullptr;
    }

    T& operator[](size_t index) const {
        return ptr_[index];
    }

    Deleter& get_deleter() {
        return *this;
    }

    const Deleter& get_deleter() const {
        return *this;
    }

    // =================== Modifiers ================================
    T* release() {
        T* released_ptr = ptr_;
        ptr_ = nullptr;
        return released_ptr;
    }

    void reset(T* ptr = nullptr) {
        if (ptr_ != ptr) {
            if (ptr_) {
                get_deleter()(ptr_);
            }
            ptr_ = ptr;
        }
    }

    void swap(UniquePtr& other) noexcept {
        using std::swap;
        swap(ptr_, other.ptr_);
        swap(get_deleter(), other.get_deleter());
    }

private:
    T* ptr_;
};

// =================== Функция make_unique ====================================
template <typename T, typename... Args>
UniquePtr<T> make_unique(Args&&... args) {
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}