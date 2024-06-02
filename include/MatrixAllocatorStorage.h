#include <memory>

template <typename T>
class MatrixAllocatorStorage {
    using pointer = T *;
    using storagePointer = MatrixAllocatorStorage<T> *;

   private:
    storagePointer nextAllocatedArea;
    pointer allocatedArea;
    size_t allocatedSize;

   public:
    MatrixAllocatorStorage() = delete;
    MatrixAllocatorStorage(const MatrixAllocatorStorage &) = delete;
    MatrixAllocatorStorage &operator=(const MatrixAllocatorStorage &) = delete;

    MatrixAllocatorStorage(size_t);
    MatrixAllocatorStorage(MatrixAllocatorStorage &&) = default;
    MatrixAllocatorStorage &operator=(MatrixAllocatorStorage &&) = default;
    allocateArea(size_t n, const void *hint = 0);
    T *getPointerToArea();
    storagePointer *getPointerToNext();
    T *GetSizeOfNext();
    deallocateArea(T *p, size_t n);

    template <class U, class... Args>
    void constructInArea(U *p, Args &&...args);

    template <class U>
    void destroyInArea(U *p);
};

template <typename T>
MatrixAllocatorStorage<T>::MatrixAllocatorStorage(
    size_t requestedAllocationSpace) {
    allocatedArea = new T[requestedAllocationSpace];
    nextAllocatedArea = nullptr;
    allocatedSize = requestedAllocationSpace;
}

template <typename T>
T *MatrixAllocatorStorage<T>::allocateArea(size_t n, const void *hint = 0) {
    nextAllocatedArea = new MatrixAllocatorStorage<T>(n);
    return nextAllocatedArea->allocatedArea;
}

template <typename T>
void MatrixAllocatorStorage<T>::deallocateArea(T *p, size_t n) {
    if (p == nextAllocatedArea) {
        storagePointer temporaryNextAllocatedArea =
            nextAllocatedArea->getPointerToNext();
        pointer temporaryNextArea = nextAllocatedArea->getPointerToArea();
        // n == nextAllocatedArea->GetSizeOfNext() needs to be true to free the
        // occupied memory
        if (temporaryNextArea != nullptr) {
            delete[] temporaryNextArea;
            delete[] nextAllocatedArea;
            nextAllocatedArea = temporaryNextAllocatedArea;
        }
    } else {
        nextAllocatedArea->deallocateArea(p, n);
    }
}

template <typename T>
template <class U, class... Args>
void MatrixAllocatorStorage<T>::constructInArea(U *p, Args &&...args);

template <typename T>
template <class U>
void MatrixAllocatorStorage<T>::destroyInArea(U *p);

template <typename T>
T *getPointerToArea() {
    return allocatedArea;
}

template <typename T>
T *getPointerToNext() {
    return nextAllocatedArea;
}

template <typename T>
T *GetSizeOfNext() {
    return allocatedSize;
}