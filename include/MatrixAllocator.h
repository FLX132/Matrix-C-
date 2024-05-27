#include <cstddef>
#include <memory>
#include <type_traits>

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
        typedef allocator<U> other;
    };

   public:
    MatrixAllocator() noexcept;
    MatrixAllocator(const MatrixAllocator &other) noexcept;
    template <class U>
    MatrixAllocator(const MatrixAllocator<U> &other) noexcept;
    ~MatrixAllocator();

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

template <class T1, class T2>
bool operator==(const allocator<T1> &lhs, const allocator<T2> &rhs) noexcept;

template <class T1, class T2>
bool operator!=(const allocator<T1> &lhs, const allocator<T2> &rhs) noexcept;