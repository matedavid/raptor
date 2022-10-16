#pragma once

#include <memory>

namespace liquid {

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T>
using Scpe = std::unique_ptr<T>;

}