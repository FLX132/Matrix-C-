#include <cstddef>
#include <memory>
#include <type_traits>

#include "MatrixAllocatorStorage.h"

template <typename T>
class MatrixAllocator {
    using value_type = T;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using propagate_on_container_move_assignment = std::true_type;
    using is_always_equal = std::true_type;

    template <class U>
    struct rebind {
        typedef MatrixAllocator<U> other;
    };

   private:
    MatrixAllocatorStorage<T> *firstAllocatedStorage;

   public:
    MatrixAllocator() noexcept = default;
    MatrixAllocator(const MatrixAllocator &other) noexcept = default;

    template <class U>
    MatrixAllocator(const MatrixAllocator<U> &other) noexcept;
    ~MatrixAllocator() = default;

    pointer address(reference x) const noexcept;

    const_pointer address(const_reference x) const noexcept;

    pointer allocate(size_type n, const void *hint = 0);

    void deallocate(T *p, std::size_t n);

    size_type max_size() const noexcept;

    template <class U, class... Args>
    void construct(U *p, Args &&...args);

    template <class U>
    void destroy(U *p);
};

template <typename T>
template <class U>
MatrixAllocator<T>::MatrixAllocator(const MatrixAllocator<U> &other) noexcept {
    /// TODO: (felix)
}

template <typename T>
typename MatrixAllocator<T>::pointer MatrixAllocator<T>::address(
    reference x) const noexcept {
    return std::addressof(x);
}

template <typename T>
typename MatrixAllocator<T>::const_pointer MatrixAllocator<T>::address(
    const_reference x) const noexcept {
    return std::addressof(x);
}

template <typename T>
typename MatrixAllocator<T>::pointer MatrixAllocator<T>::allocate(
    size_type n, const void *hint) {
    if (firstAllocatedStorage == nullptr) {
        firstAllocatedStorage = new MatrixAllocatorStorage<T>(n);
        return firstAllocatedStorage->allocatedArea;
    } else {
        return firstAllocatedStorage->allocateArea(n, hint);
    }
}

template <typename T>
void MatrixAllocator<T>::deallocate(T *p, std::size_t n) {
    if (firstAllocatedStorage != nullptr) {
        firstAllocatedStorage->deallocateArea(p, n);
    }
}

template <typename T>
typename MatrixAllocator<T>::size_type MatrixAllocator<T>::max_size()
    const noexcept {
    return std::numeric_limits<size_type>::max() / sizeof(value_type);
}

template <typename T>
template <class U, class... Args>
void MatrixAllocator<T>::construct(U *p, Args &&...args) {   /// TODO
}

template <typename T>
template <class U>
void MatrixAllocator<T>::destroy(U *p) {   /// TODO
}

template <class T1, class T2>
bool operator==(const MatrixAllocator<T1> &lhs,
                const MatrixAllocator<T2> &rhs) noexcept {
    return true;
}

template <class T1, class T2>
bool operator!=(const MatrixAllocator<T1> &lhs,
                const MatrixAllocator<T2> &rhs) noexcept {
    return true;
}