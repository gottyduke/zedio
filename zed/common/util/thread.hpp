#pragma once

#include "zed/common/util/noncopyable.hpp"
// Linux
#include <pthread.h>
#include <unistd.h>
// C
#include <cstring>
// C++
#include <string_view>
#include <thread>

namespace zed::current_thread {

auto get_tid() noexcept -> pid_t {
    static thread_local pid_t t_id = ::gettid();
    return t_id;
}

static thread_local char t_name[64]{};

auto set_thread_name(const std::string_view &str) {
    memcpy(t_name, str.data(), std::min(sizeof(t_name), str.length()));
}

auto get_thread_name() -> std::string_view {
    return t_name;
}

} // namespace zed::current_thread

namespace zed::util {
class SpinMutex : Noncopyable {
public:
    explicit SpinMutex(int pshared = 0) noexcept { pthread_spin_init(&mutex_, pshared); }
    ~SpinMutex() noexcept { pthread_spin_destroy(&mutex_); }

    void lock() noexcept { pthread_spin_lock(&mutex_); }
    void unlock() noexcept { pthread_spin_unlock(&mutex_); }

private:
    pthread_spinlock_t mutex_;
};

} // namespace zed::util
