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

    constexpr Image(int width, int height) : _width {width}, _height {height}
    {
        buf.resize(width * height);
    }

    constexpr int  width() const { return _width; }

    constexpr int  height() const { return _height; }

    constexpr void put_pixel(int x, int y, Color color)
    {
        std::size_t idx = y * _width + x;
        if (idx < 0 || idx > buf.size()) {
            return;
        }
        // index = y * width + x
        buf[idx] = color;
    }

    constexpr Color get_pixel(int x, int y) const
    {
        std::size_t idx = y * _width + x;
        if (idx < 0 || idx > buf.size()) {
            return {};
        }
        return buf[idx];
    }

    void render(std::ostream &out);

private:
    int                _width;
    int                _height;
    std::vector<Color> buf;
};

}    // namespace cacs305