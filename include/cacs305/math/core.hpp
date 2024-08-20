#pragma once

#include <concepts>
#include <limits>
#include <numbers>

namespace cacs305::math {

struct Degree
{
    double           angle;

    constexpr double radians() const
    {
        using std::numbers::pi;
        // x deg = x * pi / 180
        return angle * pi / 180;
    }
};

namespace _impl__detail {

constexpr double sqrtNewtonRaphson(double x, double curr, double prev)
{
    return curr == prev ? curr
                        : sqrtNewtonRaphson(x, 0.5f * (curr + x / curr), curr);
}

}    // namespace _impl__detail

/*
* Constexpr version of the square root
* Return value:
*	- For a finite and non-negative value of "x", returns an approximation for the square root of "x"
*   - Otherwise, returns NaN
*/
constexpr double sqrt(double x)
{
    return x >= 0 && x < std::numeric_limits<double>::infinity()
             ? _impl__detail::sqrtNewtonRaphson(x, x, 0)
             : std::numeric_limits<double>::quiet_NaN();
}

template<typename T, std::integral U>
T constexpr pow(T base, U exponent)
{
    return exponent == 0 ? 1 : base * pow(base, exponent - 1);
}

}    // namespace cacs305::math