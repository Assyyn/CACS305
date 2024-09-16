// expanding upon SimpleCircle.cpp

#include "cacs305/utilities.hpp"
#include <cacs305/image.hpp>
#include <cacs305/math/core.hpp>
#include <cmath>
#include <fstream>
#include <string>

using namespace cacs305;

std::vector<Point2D> circle_points_sym8(float radius, Point2D center = {0, 0})
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

    we utilise the 8 way symmetry to generate 7 points from x and y!

    */
    std::vector<Point2D> points;
    const auto           r2 = radius * radius;

    points.emplace_back(
        Point2D {center.x, narrow_cast<int>(center.y + radius)});
    points.emplace_back(
        Point2D {center.x, narrow_cast<int>(center.y - radius)});
    for (int x = 1; x < radius; ++x) {
        int y = std::round(math::sqrt(r2 - x * x));
        points.emplace_back(Point2D {center.x + x, center.y + y});
        points.emplace_back(Point2D {center.x + x, center.y - y});
        points.emplace_back(Point2D {center.x - x, center.y + y});
        points.emplace_back(Point2D {center.x - x, center.y - y});
        points.emplace_back(Point2D {center.x + y, center.y + x});
        points.emplace_back(Point2D {center.x + y, center.y - x});
        points.emplace_back(Point2D {center.x - y, center.y + x});
        points.emplace_back(Point2D {center.x - y, center.y - x});
    }

    points.emplace_back(
        Point2D {center.x, narrow_cast<int>(center.y + radius)});
    points.emplace_back(
        Point2D {center.x, narrow_cast<int>(center.y - radius)});
    points.emplace_back(
        Point2D {narrow_cast<int>(center.x + radius), center.y});
    points.emplace_back(
        Point2D {narrow_cast<int>(center.x - radius), center.y});

    return points;
}

void draw_circle_sym8(Image &image, float radius, Point2D center, Color color)
{
    for (const auto &point : circle_points_sym8(radius, center)) {
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

    for (int thickness = 100; thickness > 0;
         --thickness) {    // there must be a better solution though!
        draw_circle_sym8(image,
                         radius + thickness,
                         {img_width / 2, img_height / 2},
                         {200, 100, 0});
    }

    std::ofstream circle {"circle.ppm"}; // we have a donut, cheers!
    image.render(circle);
}