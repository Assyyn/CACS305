#include <cacs305/image.hpp>

#include <cmath>
#include <fstream>

using namespace cacs305;

void draw_line_DDA(Image  &image,
                   Point2D start,
                   Point2D end,
                   Color   color = {.red = 255, .green = 255, .blue = 255})
{
    if (start == end) {
        image.put_pixel(start, color);
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
        image.put_pixel(std::round(x), std::round(y), color);
    }
}

int main()
{
    constexpr int image_height = 200;
    constexpr int image_width  = 200;
    Image         image {image_width, image_height};

    draw_line_DDA(image, {0, 0}, {image_width, image_height});
    draw_line_DDA(image, {image_width, 0}, {0, image_height});
    draw_line_DDA(image, {image_width / 2, 0}, {image_width / 2, image_height});

    std::ofstream file("DDA.ppm");
    image.render(file);
}