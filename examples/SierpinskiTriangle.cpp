/*
    2024-08-03 github.com/Assyyn

    Drawing a Sierpinski Triangle with the chaos method
*/

#include <cacs305/image.hpp>

#include <random>
#include <fstream>
#include <array>
#include <limits>

using namespace cacs305;

namespace {

using Triangle = std::array<Point2D, 3>;

static std::mt19937 &engine()
{
    static std::random_device rd {};
    static std::mt19937       engine {rd()};

    return engine;
}

static Color image_color(const Image &image, const Point2D &point)
{
    constexpr static Color red {.red = 255};
    constexpr static Color blue {.blue = 255};
    constexpr static Color green {.green = 255};

    if (point.y < image.height() / 2) {
        // point is in the top quadrant
        return red;
    }

    if ((point.x < image.width() / 2) && (point.y > image.height() / 2)) {
        // bottom left quadrant
        return green;
    }

    return blue;    // blue bottom right
}

static const Point2D &random_corner(const Triangle &triangle)
{
    std::uniform_int_distribution dist {0, 2};
    return triangle[dist(engine())];
}

static void move_point_halfway(Point2D &point, const Point2D &towards)
{
    point.x = (point.x + towards.x) / 2;
    point.y = (point.y + towards.y) / 2;
}

static void draw_triangle(Image         &image,
                          const Point2D &vertex_A,
                          const Color   &color = {255, 255, 255})
{
    const Point2D vertex_B {vertex_A.x + 2, vertex_A.y};
    const Point2D vertex_C {vertex_A.x + 1, vertex_A.y - 1};

    image.put_pixel(vertex_A.x, vertex_A.y, color);
    image.put_pixel(vertex_A.x + 1, vertex_A.y, color);
    image.put_pixel(vertex_B.x, vertex_C.y, color);
    image.put_pixel(vertex_C.x, vertex_C.y, color);
}

static void draw_sierpienski(Image          &image,
                             const Triangle &triangle,
                             std::size_t     iterations = 0xffffff)
{
    Point2D current {random_corner(triangle)};

    for (std::size_t i = 0; i < iterations; ++i) {
        move_point_halfway(current, random_corner(triangle));
        draw_triangle(image, current, image_color(image, current));
    }
}

}    // namespace

int main()
{
    constexpr int     image_width  = 1'920;
    constexpr int     image_height = 1'080;

    // bounding vertices of the sierpienski triangle
    constexpr Point2D vertex_A {
        .x = 0,
        .y = image_height,
    };
    constexpr Point2D vertex_B {
        .x = image_width,
        .y = image_height,
    };
    constexpr Point2D vertex_C {
        .x = image_width / 2,
        .y = 0,
    };

    constexpr Triangle triangle {vertex_A, vertex_B, vertex_C};

    Image              image {image_width, image_height};

    draw_sierpienski(image, triangle);

    std::ofstream file("sierpinski.ppm");
    image.render(file);
}