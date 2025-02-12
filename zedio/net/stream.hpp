#pragma once

#include "zedio/net/io.hpp"

namespace zedio::net::detail {

template <class Stream, class Addr>
class BaseStream {
protected:
    BaseStream(detail::SocketIO &&io)
        : io_{std::move(io)} {}

public:
    [[nodiscard]]
    auto shutdown(io::ShutdownBehavior how) noexcept {
        return io_.shutdown(how);
    }

    [[nodiscard]]
    auto close() noexcept {
        return io_.close();
    }

    [[nodiscard]]
    auto read(std::span<char> buf) const noexcept {
        return io_.recv(buf);
    }

    template <typename... Ts>
    [[nodiscard]]
    auto read_vectored(Ts &...bufs) const noexcept {
        return io_.read_vectored(bufs...);
    }

    [[nodiscard]]
    auto write(std::span<const char> buf) noexcept {
        return io_.send(buf);
    }

    template <typename... Ts>
    [[nodiscard]]
    auto write_vectored(Ts &...bufs) noexcept {
        return io_.write_vectored(bufs...);
    }

    [[nodiscard]]
    auto write_all(std::span<const char> buf) noexcept {
        return io_.write_all(buf);
    }

    [[nodiscard]]
    auto local_addr() const noexcept {
        return io_.local_addr<Addr>();
    }

    [[nodiscard]]
    auto peer_addr() const noexcept {
        return io_.peer_addr<Addr>();
    }

    [[nodiscard]]
    auto fd() const noexcept {
        return io_.fd();
    }

public:
    [[nodiscard]]
    static auto connect(const Addr &addr) {
        return detail::SocketIO::build_stream<Stream, Addr>(addr);
    };

protected:
    detail::SocketIO io_;
};

} // namespace zedio::net::detail
