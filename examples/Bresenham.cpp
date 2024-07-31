#include <cacs305/image.hpp>

#include <fstream>
#include <cmath>

using namespace cacs305;

namespace {

static void
_bresenhams_low(Image &image, Point2D start, Point2D end, Color color)
{
    const int dx     = end.x - start.x;
    const int dy     = end.y - start.y;
    const int y_incr = (dy < 0) ? -1 : 1;
    const int abs_dy = std::abs(dy);

    int       diff   = (2 * abs_dy) - dx;
    int       y      = start.y;


    for (int x = start.x; x <= end.x; ++x) {
        image.put_pixel(x, y, color);

        diff += 2 * abs_dy;
        if (diff > 0) {
            y    += y_incr;
            diff -= 2 * dx;
        }
    }
}

static void
_bresenhams_high(Image &image, Point2D start, Point2D end, Color color)
{
    const int dx     = end.x - start.x;
    const int dy     = end.y - start.y;

    const int x_incr = (dx < 0) ? -1 : 1;
    const int abs_dx = std::abs(dx);

    int       diff   = 2 * abs_dx - dy;
    int       x      = start.x;

    for (int y = start.y; y <= end.y; ++y) {
        image.put_pixel(x, y, color);

        diff += 2 * abs_dx;
        if (diff > 0) {
            x    += x_incr;
            diff -= 2 * dy;
        }
    }
}

}    // namespace

void draw_line_Bresenhams(Image  &image,
                          Point2D start,
                          Point2D end,
                          Color color = {.red = 255, .green = 255, .blue = 255})
{
    const int dx = end.x - start.x;
    const int dy = end.y - start.y;

    if (std::abs(dy) < std::abs(dx)) {
        if (start.x > end.x) {
            std::swap(start, end);
        }

        _bresenhams_low(image, start, end, color);
    } else {
        if (start.y > end.y) {
            std::swap(start, end);
        }
        _bresenhams_high(image, start, end, color);
    }
}

int main()
{
    constexpr int image_height = 200;
    constexpr int image_width  = 200;
    Image         image {image_width, image_height};

    draw_line_Bresenhams(image,
                         {0, image_height / 2},
                         {image_width, image_height / 2});

    std::ofstream file("Bresenham.ppm");
    image.render(file);
}