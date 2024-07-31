#include <cacs305/image.hpp>

#include <fstream>
#include <cmath>

using namespace cacs305;

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

void flood_fill_4_way(Image &image, Point2D point, Color fill, Color interior)
{
    // TODO: use stack or queue instead of recursion
    if (point.x < 0 || point.y < 0 || point.x >= image.width()
        || point.y >= image.height()
        || image.get_pixel(point.x, point.y) != interior) {
        return;
    }
    image.put_pixel(point.x, point.y, fill);

    flood_fill_4_way(image, {point.x + 1, point.y}, fill, interior);
    flood_fill_4_way(image, {point.x - 1, point.y}, fill, interior);
    flood_fill_4_way(image, {point.x, point.y + 1}, fill, interior);
    flood_fill_4_way(image, {point.x, point.y - 1}, fill, interior);
}

int main()
{
    constexpr int image_height = 200;
    constexpr int image_width  = 200;
    Image         image {image_width, image_height};

    draw_line_DDA(image,
                  {0, image_height / 2},
                  {image_width, image_height / 2});
    flood_fill_4_way(image, {1, 0}, Color {.red = 255}, Color {});

    std::ofstream file("FloodFill4way.ppm");
    image.render(file);
}