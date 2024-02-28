#pragma once

#include "zedio/common/concepts.hpp"
#include "zedio/io/time/timer.hpp"

namespace zedio::io::detail {

// requires is_awaiter<IO>
template <typename IO>
class TimeoutIO : public IO {
public:
    TimeoutIO(IO &&io, const std::chrono::nanoseconds &timeout)
        : IO{std::move(io)}
        , timeout_{timeout} {}

    auto await_suspend(std::coroutine_handle<> handle) -> bool {
        event_handle_ = t_timer->add_timer_event([this]() { cancel_op(); }, timeout_);
        return IO::await_suspend(handle);
    }

    auto await_resume() -> decltype(IO::await_resume()) {
        event_handle_->cancel();
        return IO::await_resume();
    }

private:
    void cancel_op() {
        auto sqe = detail::t_poller->get_sqe();
        if (sqe != nullptr) [[likely]] {
            io_uring_prep_cancel(sqe, &this->cb_, 0);
            io_uring_sqe_set_data(sqe, nullptr);
            if (auto ret = detail::t_poller->submit(); ret < 0) [[unlikely]] {
                LOG_ERROR("cancel io failed, error: {}", strerror(-ret));
            }
        }
    }

private:
    std::chrono::nanoseconds    timeout_;
    std::shared_ptr<TimerEvent> event_handle_{nullptr};
};

} // namespace zedio::io::detail