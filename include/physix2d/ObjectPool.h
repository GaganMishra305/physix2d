#pragma once

#include <vector>
#include <cstddef>

namespace physix2d {

// A simple fixed-chunk free-list object pool. Recycles freed slots so bulk,
// churny objects (particles, debris, contacts) don't hammer the allocator.
// Pointers stay stable: storage grows in chunks that are never reallocated.
template <class T, size_t ChunkSize = 1024>
class ObjectPool {
public:
    ObjectPool() = default;
    ~ObjectPool() { for (T* c : chunks_) ::operator delete[](c); }

    ObjectPool(const ObjectPool&) = delete;
    ObjectPool& operator=(const ObjectPool&) = delete;

    // Allocate + construct.
    template <class... Args>
    T* create(Args&&... args) {
        T* slot;
        if (!freeList_.empty()) {
            slot = freeList_.back();
            freeList_.pop_back();
        } else {
            if (cursor_ == ChunkSize || chunks_.empty()) {
                chunks_.push_back(static_cast<T*>(::operator new[](sizeof(T) * ChunkSize)));
                cursor_ = 0;
            }
            slot = chunks_.back() + cursor_++;
        }
        ++liveCount_;
        return new (slot) T(static_cast<Args&&>(args)...);
    }

    // Destruct + recycle.
    void destroy(T* p) {
        if (!p) return;
        p->~T();
        freeList_.push_back(p);
        --liveCount_;
    }

    size_t liveCount() const { return liveCount_; }
    size_t capacity() const { return chunks_.size() * ChunkSize; }

private:
    std::vector<T*> chunks_;
    std::vector<T*> freeList_;
    size_t cursor_ = ChunkSize; // force a fresh chunk on first create
    size_t liveCount_ = 0;
};

}
