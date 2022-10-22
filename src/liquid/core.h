#pragma once

#include <memory>

namespace liquid {

template <typename T>
using Ref = std::shared_ptr<T>;

#define MakeRef(T, ...) std::make_shared<T>(__VA_ARGS__)

template <typename T>
using Scope = std::unique_ptr<T>;

#define MakeScope(T, ...) std::make_unique<T>(__VA_ARGS__)

}