#pragma once

#include <memory>
#include "spdlog/spdlog.h"

namespace liquid {

#define LOG_INFO(...)  spdlog::info(__VA_ARGS__)
#define LOG_WARN(...)  spdlog::warn(__VA_ARGS__)
#define LOG_ERROR(...) spdlog::error(__VA_ARGS__)

template<typename T>
using Ref = std::shared_ptr<T>;

#define MakeRef(T, ...) std::make_shared<T>(__VA_ARGS__)

template<typename T>
using Scope = std::unique_ptr<T>;

#define MakeScope(T, ...) std::make_unique<T>(__VA_ARGS__)

} // namespace liquid