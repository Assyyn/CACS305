#include <cacs305/image.hpp>
#include <cacs305/math/core.hpp>
#include <cacs305/utilities.hpp>

#include <fstream>
#include <cmath>
#include <cassert>
#include <vector>
#include <queue>

using namespace cacs305;

namespace {

struct Line2D
{
    Point2D          start;
    Point2D          end;

    constexpr double length() const
    {
        return math::sqrt(math::pow(end.x - start.x, 2)
                          + math::pow(end.y - start.y, 2));
    }
};

constexpr void
rotate(Point2D &point, math::Degree angle, const Point2D &center = {})
{
    const auto radians = angle.radians();
    const auto dx      = point.x - center.x;
    const auto dy      = point.y - center.y;
    const auto cos     = std::cos(radians);
    const auto sin     = std::sin(radians);

    point.x            = dx * cos - dy * sin + center.x;
    point.y            = dx * sin + dy * cos + center.y;
}

constexpr Point2D rotate_copy(const Point2D &point,
                              math::Degree   angle,
                              const Point2D &center = {})
{
    const auto radians = angle.radians();
    const auto dx      = point.x - center.x;
    const auto dy      = point.y - center.y;
    const auto cos     = std::cos(radians);
    const auto sin     = std::sin(radians);

    return {narrow_cast<int>(dx * cos - dy * sin + center.x),
            narrow_cast<int>(dx * sin + dy * cos + center.y)};
}


std::vector<Point2D> line_points(const Line2D &line)
{
    // use DDA

    std::vector<Point2D> points;
    const auto           dx    = line.end.x - line.start.x;
    const auto           dy    = line.end.y - line.start.y;

    const auto           n     = std::max(std::abs(dx), std::abs(dy));
    const auto           div_n = (n == 0) ? 0.0f : 1.0f / n;

    const auto           xstep = dx * div_n;
    const auto           ystep = dy * div_n;

    float                x     = line.start.x;
    float                y     = line.start.y;
    for (auto step = 0; step <= n; step++) {
        points.emplace_back(Point2D {narrow_cast<int>(std::round(x)),
                                     narrow_cast<int>(std::round(y))});
        x += xstep;
        y += ystep;
    }

    return points;
}

void draw_line(Image &image, const Line2D &line, Color color = {255, 255, 255})
{
    for (const auto point : line_points(line)) {
        image.put_pixel(point.x, point.y, color);
    }
}

using Canopy = std::vector<Line2D>;

struct BranchChildren
{
    Line2D first;
    Line2D second;
};

BranchChildren
gen_children(const Line2D &root, math::Degree angle, double scale_factor)
{
    auto extend_line_endpoint = [](const Line2D &line,
                                   double        length) -> Point2D {
        const auto len = line.length();
        const auto dx  = line.end.x - line.start.x;
        const auto dy  = line.end.y - line.start.y;

        return {
            narrow_cast<int>(line.end.x + dx / len * length),
            narrow_cast<int>(line.end.y + dy / len * length),
        };
    };

    const auto initial_endpoint =
        extend_line_endpoint(root, root.length() * scale_factor);

    return BranchChildren {
        .first  = {root.end,rotate_copy(initial_endpoint,angle, root.end)         },
        .second = {root.end,
                   rotate_copy(initial_endpoint,
                   {360 - angle.angle},
                   root.end)},
    };
}

Canopy gen_canopy(Line2D       root,
                  math::Degree angle,
                  double       scale_factor = 3.0 / 4,
                  double       min_length   = 1)
{
    // start with a parent.
    // push it to the vector.
    // generate the child branches.
    // do the same with the child branches individually.
    // keep going while the branches are larger than min_length.

    Canopy canopy;
    {
        auto iterations = 0;
        auto len        = root.length();
        while (len >= min_length) {
            len *= scale_factor;
            iterations++;
        }

        canopy.reserve(iterations);
    }
    std::queue<Line2D> queue;
    queue.push(root);

    while (!queue.empty()) {
        const auto current = queue.front();
        queue.pop();

        canopy.push_back(current);

        if (current.length() < min_length) {    // abort when too small
            break;
        }

        const auto [child_A, child_B] =
            gen_children(current, angle, scale_factor);
        queue.push(child_A);
        queue.push(child_B);
    }

    return canopy;
}

}    // namespace

int main()
{
    constexpr int    image_width  = 1'920;
    constexpr int    image_height = 1'080;

    Image            image {image_width, image_height};

    constexpr Line2D base {
        .start = {image_width / 2,          image_height},
        .end   = {image_width / 2, image_height * 8 / 10},
    };

    for (const auto &line : gen_canopy(base, {20})) {
        draw_line(image, line);
    }

    std::ofstream file {"FractalCanopy.ppm"};
    image.render(file);
}