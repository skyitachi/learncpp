//
// Created by skyitachi on 24-1-23.
//

#include <atomic>
#include <iostream>
#include <thread>
#include <vector>
#include <type_traits>

constexpr uint64_t LOCK_FLAG = 1L >> 63;

#define	SPINLOCK_BACKOFF_MIN	4
#define	SPINLOCK_BACKOFF_MAX	128
#if defined(__x86_64__) || defined(__i386__)
#define SPINLOCK_BACKOFF_HOOK	__asm volatile("pause" ::: "memory")
#else
#define SPINLOCK_BACKOFF_HOOK
#endif
#define	SPINLOCK_BACKOFF(count) \

class Node {
public:
    Node(): data_(0) {}
    Node(uint64_t data, uint64_t lock): data_(data), lock_(lock) {}

    explicit Node(uint64_t data): data_(data) {}

//    Node(const Node& other): data_(other.data_), lock_(0) {}
//
//    Node(Node &&other) noexcept : data_(other.data_), lock_(0) {}
//
//    Node &operator=(const Node& other) {
//        if (this != &other) {
//            data_ = other.data_;
//            lock_ = 0;
//        }
//        return *this;
//    }
//
//    Node &operator=(Node &&other) noexcept {
//        if (this != &other) {
//            data_ = other.data_;
//            lock_ = 0;
//        }
//        return *this;
//    }
//
//    Node load() const {
//        return {data_, lock_.load()};
//    }
//
//    void store(const Node &value) {
//        data_ = value.data_;
//        lock_.store(value.lock_);
//    }
    void shared_lock() {

    }

public:
    uint64_t data_;
    uint64_t lock_ = {0};
};

void unlock(std::atomic<uint64_t>  &node) {
    uint64_t s = node.load(std::memory_order_relaxed) & ~LOCK_FLAG;

    node.store(s, std::memory_order_release);
}

void lock_node(std::atomic<Node> &node) {

    unsigned bcount = 4;
    Node s = Node(0);
again:
    s = std::atomic_load(&node);
    if (s.data_ & LOCK_FLAG) {
        SPINLOCK_BACKOFF(bcount);
        goto again;
    }
    /* Acquire from prior release in unlock_node.() */
    Node newS(s.data_ | LOCK_FLAG);
    if (!node.compare_exchange_weak(s, newS)) {
        bcount = SPINLOCK_BACKOFF_MIN;
        goto again;
    }

}

void unlock_node(std::atomic<Node>  &node) {
    uint64_t s = node.load(std::memory_order_relaxed).data_ & ~LOCK_FLAG;

    node.store(Node(s), std::memory_order_release);
}

void shared_lock_node(std::atomic<Node> &node) {
    do {
        Node n = node.load(std::memory_order_acquire);
        if (n.lock_ & LOCK_FLAG) {
            std::this_thread::yield();

        } else {
            break;
        }
    } while(true);

    do {
        Node n = node.load(std::memory_order_acquire);

    } while (true);

}

void lock(std::atomic<uint64_t>& node) {
    unsigned bcount = 4;
    uint64_t s;
again:
    s = node.load();
    if (s & LOCK_FLAG) {
        SPINLOCK_BACKOFF(bcount);
        goto again;
    }
    /* Acquire from prior release in unlock_node.() */
    if (!node.compare_exchange_weak(s, s | LOCK_FLAG)) {
        bcount = SPINLOCK_BACKOFF_MIN;
        goto again;
    }
}


void lock_demo() {
    std::atomic<uint64_t> node = {0};
    int g_count = 0;

    std::cout << " sizeof(std::atomic<uint64_t>) = " << sizeof(node) <<std::endl;
    std::cout << " sizeof(std::atomic<uint32_t>) = " << sizeof(std::atomic<uint32_t>) <<std::endl;

    std::vector<std::thread> ths;

    for (int i = 0; i < 10; i++) {
        ths.emplace_back([&] {
            for (int i = 0; i < 10000; i++) {
                lock(node);
                g_count++;
                unlock(node);
            }
        });

    }

    for(auto &th: ths) {
        th.join();
    }

    std::cout << "final count: " <<g_count << std::endl;

}

void lock_node_demo() {
    std::atomic<Node> node;
    int g_count = 0;

    std::vector<std::thread> ths;

    for (int i = 0; i < 10; i++) {
        ths.emplace_back([&] {
            for (int i = 0; i < 10000; i++) {
                lock_node(node);
                g_count++;
                unlock_node(node);
            }
        });

    }

    for(auto &th: ths) {
        th.join();
    }

    std::cout << "lock by node final count: " <<g_count << std::endl;
}

class rw_spin_lock
{
public:
    rw_spin_lock()
    {
        _readers = 0;
    }

public:
    void acquire_reader()
    {
        int retry = 0;
        while (true)
        {
            uint32_t prev_readers = _readers;
            if (prev_readers != HAS_WRITER)
            {
                uint32_t new_readers = prev_readers + 1;
                if (_readers.compare_exchange_weak(prev_readers, new_readers))
                {
                    // we've won the race
                    return;
                }
            }

            retry++;
            if (retry > RETRY_THRESHOLD)
            {
                retry = 0;
                std::this_thread::yield();
            }
        }
    }

    void release_reader()
    {
        int retry = 0;
        while (true)
        {
            uint32_t prev_readers = _readers;
            if (prev_readers != HAS_WRITER && prev_readers > 0)
            {
                uint32_t new_readers = prev_readers - 1;
                if (_readers.compare_exchange_weak(prev_readers, new_readers))
                {
                    // we've won the race
                    return;
                }
            }

            retry++;
            if (retry > RETRY_THRESHOLD)
            {
                retry = 0;
                std::this_thread::yield();
            }
        }
    }

    void acquire_writer()
    {
        int retry = 0;
        while (true)
        {
            uint32_t prev_readers = _readers;
            if (prev_readers == 0)
            {
                if (_readers.compare_exchange_weak(prev_readers, HAS_WRITER))
                {
                    // we've won the race
                    return;
                }
            }

            retry++;
            if (retry > RETRY_THRESHOLD)
            {
                // save some cpu cycles
                retry = 0;
                std::this_thread::yield();
            }
        }
    }

    void release_writer()
    {
        int retry = 0;
        while (true)
        {
            uint32_t prev_readers = _readers;
            if (prev_readers == HAS_WRITER)
            {
                if (_readers.compare_exchange_weak(prev_readers, 0))
                {
                    // we've won the race
                    return;
                }
            }

            retry++;
            if (retry > RETRY_THRESHOLD)
            {
                // save some cpu cycles
                retry = 0;
                std::this_thread::yield();
            }
        }
    }

private:
    const uint32_t HAS_WRITER = 0xffffffff;
    const int RETRY_THRESHOLD = 100;
    std::atomic<uint32_t> _readers;
};

class reader_lock
{
public:
    reader_lock(rw_spin_lock &lock) : _lock(lock)
    {
        _lock.acquire_reader();
    }

    ~reader_lock()
    {
        _lock.release_reader();
    }

private:
    rw_spin_lock &_lock;
};

class writer_lock
{
public:
    explicit writer_lock(rw_spin_lock &lock) : _lock(lock)
    {
        _lock.acquire_writer();
    }

    ~writer_lock()
    {
        _lock.release_writer();
    }

private:
    rw_spin_lock &_lock;
};

void reader_writer_lock_demo() {
    rw_spin_lock lock;
    int g_count = 0;

    std::cout << " sizeof(std::atomic<uint64_t>) = " << sizeof(lock) <<std::endl;
    std::cout << " sizeof(std::atomic<uint32_t>) = " << sizeof(std::atomic<uint32_t>) <<std::endl;

    std::vector<std::thread> ths;

    for (int i = 0; i < 20; i++) {
        ths.emplace_back([&] {
            for (int i = 0; i < 10000; i++) {
                writer_lock lock_guard(lock);
                g_count++;
            }
        });

    }

    for(auto &th: ths) {
        th.join();
    }

    std::cout << "final count: " <<g_count << std::endl;

}



int main() {

    std::atomic<bool> lock{false};

    std::cout << lock.load() << std::endl;

    auto ret = std::is_trivially_copyable<Node>::value;

    std::cout << "trivially_copyable: " << ret << std::endl;
    ret = std::is_copy_constructible<Node>::value;
    std::cout << "is_copy_constructible: " << ret << std::endl;
    ret = std::is_move_constructible<Node>::value;
    std::cout << "is_move_constructible: " << ret << std::endl;
    ret = std::is_copy_assignable<Node>::value;
    std::cout << "is_copy_assignable: " << ret << std::endl;
    ret = std::is_move_assignable<Node>::value;

    std::cout << "is_move_assignable: " << ret << std::endl;

//    lock_demo();

    reader_writer_lock_demo();
    std::atomic<int> counter = {0};
    counter.fetch_add(1);

    // 没有fetch add的功能
    std::atomic<Node> n_counter;
}
