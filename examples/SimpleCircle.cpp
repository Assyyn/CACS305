#include "cacs305/utilities.hpp"
#include <cacs305/image.hpp>
#include <cacs305/math/core.hpp>
#include <fstream>
#include <string>

using namespace cacs305;

std::vector<Point2D> simple_circle_points(float radius, Point2D center = {0, 0})
{

    /*  From the Circle equation (derivation):
               2         2      2
        (x - a) + (y - b)   = r
               2                2           2
or,     (y - b)             = r   -  (x - a)
                                     2          2
or,     y - b               = sqrt (r  - (x - a) )
                                         2          2
then,   y                   = b + sqrt (r  - (x - a) )

    we utilise the symmetrical nature of the circle to generate both the upper and lower points in one loop (can be expanded upon)
    */
    std::vector<Point2D> points;
    const auto           r2 = radius * radius;

    for (int x = center.x - radius; x <= center.x + radius; ++x) {
        int y = math::sqrt(r2 - (x - center.x) * (x - center.x));
        points.emplace_back(
            Point2D {narrow_cast<float>(x), narrow_cast<float>(center.y + y)});
        points.emplace_back(
            Point2D {narrow_cast<float>(x), narrow_cast<float>(center.y - y)});
    }

    return points;
}

void draw_circle_simple(Image  &image,
                        float   radius,
                        Point2D center,
                        Color   color)
{
    for (const auto &point : simple_circle_points(radius, center)) {
        image.put_pixel(point, color);
    }
}

int main(int argc, char *argv[])
{
    constexpr int img_width  = 1'920;
    constexpr int img_height = 1'080;

    float         radius     = 400;    // default
    if (argc > 1) {
        radius = std::abs(std::stof(
            argv[1]));    // not very production friendly (you'd use argument parsing libraries to help you out)
    }

    Image image {img_width, img_height};

    draw_circle_simple(image,
                       radius,
                       {img_width / 2.f, img_height / 2.f},
                       {200, 100, 0});

    std::ofstream circle {"simple_circle.ppm"};
    image.render(circle);
}