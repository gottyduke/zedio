#pragma once

// c
#include <cinttypes>

namespace zedio::config {

/// buf
static constexpr std::size_t DEFAULT_BUF_SIZE = 8 * 1024;

/// net
static constexpr std::size_t STREAM_BUFFER_DEFAULT_SIZE{1024};
static constexpr std::size_t EXTRA_BUFFER_SUZE{1024 * 64};

} // namespace zedio::config
