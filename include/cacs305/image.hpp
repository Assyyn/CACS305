#pragma once

#include <cstddef>
#include <cstdint>
#include <format>
#include <iosfwd>
#include <string>
#include <vector>

namespace cacs305 {

struct Color
{
    using byte        = uint8_t;

    byte        red   = 0;
    byte        green = 0;
    byte        blue  = 0;
    byte        _;

    std::string to_string() const
    {
        return std::format("{} {} {}", red, green, blue);
    }

    constexpr auto operator<=>(Color const &) const = default;
};

struct Point2D
{
    int            x                                  = 0;
    int            y                                  = 0;

    constexpr auto operator<=>(Point2D const &) const = default;
};

struct Image
{

    constexpr Image(std::size_t width, std::size_t height)
        : _width {width},
          _height {height}
    {
        buf.resize(_width * _height);
    }

    constexpr std::size_t width() const { return _width; }

    constexpr std::size_t height() const { return _height; }

    constexpr std::size_t index(int x, int y) const { return y * _width + x; }

    constexpr void        put_pixel(int x, int y, Color color)
    {
        if (x < 0 || y < 0) {
            return;
        }

        std::size_t idx = index(x, y);
        if (idx > buf.size()) {
            return;
        }
        // index = y * width + x
        buf[idx] = color;
    }

    constexpr Color get_pixel(int x, int y) const
    {
        if (x < 0 || y < 0) {
            return {};
        }

        std::size_t idx = index(x, y);
        if (idx > buf.size()) {
            return {};
        }
        return buf[idx];
    }

    void        render(std::ostream &out);
    std::string render() const
    {
        std::string result = std::format("P3\n{} {}\n255\n", _width, _height);

        for (int y = 0; y < _height; ++y) {
            for (int x = 0; x < _width; ++x) {
                std::format_to(std::back_inserter(result),
                               "{}\n",
                               buf[y * _width + x].to_string());
            }
            result += '\n';
        }

        return result;
    }

private:
    std::size_t        _width;
    std::size_t        _height;
    std::vector<Color> buf;
};

}    // namespace cacs305