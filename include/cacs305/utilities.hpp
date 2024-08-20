#pragma once

#include <utility>

template<typename T, typename U>
constexpr auto narrow_cast(U &&u) noexcept -> T
{
    return static_cast<T>(std::forward<U>(u));
}