#include <memory>

template <typename T>
class MatrixAllocatorStorage {
    using pointer = T *;
    using storagePointer = MatrixAllocatorStorage<T> *;

   private:
    storagePointer nextAllocatedArea;
    pointer allocatedArea;
    size_t maxAllocatedSize;
    size_t currentAllocatedElements;

   public:
    MatrixAllocatorStorage() = delete;
    MatrixAllocatorStorage(const MatrixAllocatorStorage &) = delete;
    MatrixAllocatorStorage &operator=(const MatrixAllocatorStorage &) = delete;

    MatrixAllocatorStorage(size_t);
    MatrixAllocatorStorage(MatrixAllocatorStorage &&) = default;
    MatrixAllocatorStorage &operator=(MatrixAllocatorStorage &&) = default;
    T *allocateArea(size_t n, const void *hint = 0);
    T *getPointerToArea();
    storagePointer getPointerToNext();
    T *GetSizeOfNext();
    void deallocateArea(T *p, size_t n);

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
    maxAllocatedSize = requestedAllocationSpace;
    currentAllocatedElements = 0;
}

template <typename T>
T *MatrixAllocatorStorage<T>::allocateArea(size_t n, const void *hint) {
    if (nextAllocatedArea == nullptr) {
        nextAllocatedArea = new MatrixAllocatorStorage<T>(n);
        return nextAllocatedArea->allocatedArea;
    } else {
        nextAllocatedArea->allocatedArea(n, hint);
    }
}

template <typename T>
void MatrixAllocatorStorage<T>::deallocateArea(T *p, size_t n) {
    if (nextAllocatedArea != nullptr) {
        if (p == nextAllocatedArea && n == nextAllocatedArea->GetSizeOfNext()) {
            storagePointer temporaryNextAllocatedArea =
                nextAllocatedArea->getPointerToNext();
            pointer temporaryNextArea = nextAllocatedArea->getPointerToArea();
            if (temporaryNextArea != nullptr) {
                delete[] temporaryNextArea;
                delete nextAllocatedArea;
                nextAllocatedArea = temporaryNextAllocatedArea;
            }
        } else {
            nextAllocatedArea->deallocateArea(p, n);
        }
    }
}

template <typename T>
template <class U, class... Args>
void MatrixAllocatorStorage<T>::constructInArea(U *p, Args &&...args) {
    auto allocatedElementsBeforeConstruct = currentAllocatedElements;
    if (currentAllocatedElements < maxAllocatedSize) {
        for (auto elementPtr : allocateArea) {
            if (elementPtr == p) {
                ::new ((void *) p) U(std::forward<Args>(args)...);
                currentAllocatedElements++;
                break;
            }
        }
    }

    if (allocatedElementsBeforeConstruct == currentAllocatedElements &&
        nextAllocatedArea != nullptr) {
        nextAllocatedArea->constructInArea(p, args...);
    }
}

template <typename T>
template <class U>
void MatrixAllocatorStorage<T>::destroyInArea(U *p) {
    auto allocatedElementsBeforeConstruct = currentAllocatedElements;
    if (currentAllocatedElements < maxAllocatedSize) {
        for (auto elementPtr : allocateArea) {
            if (elementPtr == p) {
                p->~U();
                currentAllocatedElements--;
                break;
            }
        }
    }

    if (allocatedElementsBeforeConstruct == currentAllocatedElements &&
        nextAllocatedArea != nullptr) {
        nextAllocatedArea->destroyInArea(p);
    }
}

template <typename T>
T *MatrixAllocatorStorage<T>::getPointerToArea() {
    return allocatedArea;
}

template <typename T>
typename MatrixAllocatorStorage<T>::storagePointer MatrixAllocatorStorage<
    T>::getPointerToNext() {
    return nextAllocatedArea;
}

template <typename T>
T *MatrixAllocatorStorage<T>::GetSizeOfNext() {
    return maxAllocatedSize;
}