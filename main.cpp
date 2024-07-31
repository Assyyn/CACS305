#include <cstddef>
#include <cstdint>
#include <format>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

namespace {

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

    Image(int width, int height) : _width {width}, _height {height}
    {
        buf.resize(width * height);
    }

    int  width() const { return _width; }

    int  height() const { return _height; }

    void put_pixel(int x, int y, Color color)
    {
        std::size_t idx = y * _width + x;
        if (idx < 0 || idx > buf.size()) {
            return;
        }
        // index = y * width + x
        buf[idx] = color;
    }

    Color get_pixel(int x, int y) const
    {
        std::size_t idx = y * _width + x;
        if (idx < 0 || idx > buf.size()) {
            return {};
        }
        return buf[idx];
    }

    void render(std::ostream &out)
    {
        out << "P3\n";
        out << _width << ' ' << _height << '\n';
        out << "255\n";
        std::string pixel_buf;
        for (int y = 0; y < _height; ++y) {
            for (int x = 0; x < _width; ++x) {
                pixel_buf += buf[y * _width + x].to_string() + '\n';
            }
            pixel_buf += '\n';
        }

        out << pixel_buf;
    }

private:
    int                _width;
    int                _height;
    std::vector<Color> buf;
};

}    // namespace

void draw_line_DDA(Image  &image,
                   Point2D start,
                   Point2D end,
                   Color   color = {.red = 255, .green = 255, .blue = 255})
{
    if (start == end) {
        return;
    }

    if (start > end) {
        std::swap(start, end);
    }

    const int dy    = end.y - start.y;
    const int dx    = end.x - start.x;

    float     steps = 0;
    if (std::abs(dx) > std::abs(dy)) {
        steps = std::abs(dx);
    } else {
        steps = std::abs(dy);
    }

    float const x_inc = dx / steps;
    float const y_inc = dy / steps;

    float       x     = start.x;
    float       y     = start.y;
    image.put_pixel(std::round(x), std::round(x), color);

    for (int i = 1; i <= steps; ++i) {
        x += x_inc;
        y += y_inc;
        image.put_pixel(x, y, color);
    }
}

void flood_fill_4_way(Image &image, int x, int y, Color fill, Color interior)
{
    // TODO: use stack or queue instead of recursion
    if (x < 0 || y < 0 || x >= image.width() || y >= image.height()
        || image.get_pixel(x, y) != interior) {
        return;
    }
    image.put_pixel(x, y, fill);

    flood_fill_4_way(image, x + 1, y, fill, interior);
    flood_fill_4_way(image, x - 1, y, fill, interior);
    flood_fill_4_way(image, x, y + 1, fill, interior);
    flood_fill_4_way(image, x, y - 1, fill, interior);
}

int main()
{
    int   image_height = 200;
    int   image_width  = 200;
    Image image {image_width, image_height};

    draw_line_DDA(image, {0, 0}, {image_width, image_height});
    draw_line_DDA(image, {image_width, 0}, {0, image_height});
    draw_line_DDA(image, {image_width / 2, 0}, {image_width / 2, image_height});

    constexpr Color black {};
    flood_fill_4_way(image, 24, 83, Color {.red = 255}, black);
    flood_fill_4_way(image, 130, 160, Color {.red = 255}, black);

    image.render(std::cout);
}
