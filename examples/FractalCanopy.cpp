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

    return {narrow_cast<float>(dx * cos - dy * sin + center.x),
            narrow_cast<float>(dx * sin + dy * cos + center.y)};
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
        points.emplace_back(Point2D {narrow_cast<float>(std::round(x)),
                                     narrow_cast<float>(std::round(y))});
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

BranchChildren gen_children(const Line2D      &parent,
                            const math::Degree angle,
                            const double       scale_factor)
{
    auto extend_line_endpoint = [](const Line2D &line,
                                   double        new_length) -> Point2D {
        const auto len = line.length();
        const auto dx  = line.end.x - line.start.x;
        const auto dy  = line.end.y - line.start.y;

        assert(new_length > 0);

        return {
            narrow_cast<float>(std::round(line.end.x + dx / len * new_length)),
            narrow_cast<float>(std::round(line.end.y + dy / len * new_length)),
        };
    };

    const auto initial_endpoint =
        extend_line_endpoint(parent, parent.length() * scale_factor);

    return BranchChildren {
        .first = {parent.end,rotate_copy(initial_endpoint,angle,  parent.end)        },
        .second = {parent.end,
                  rotate_copy(initial_endpoint,
                  {360 - angle.angle},
                  parent.end)},
    };
}

Canopy gen_canopy(Line2D             root,
                  const math::Degree angle,
                  const double       scale_factor,
                  std::int64_t       max_depth  = 20,
                  const double       min_length = 1)
{
    assert(scale_factor < 1);
    assert(min_length >= 1);

    // start with a parent.
    // push it to the vector.
    // generate the child branches.
    // do the same with the child branches individually.
    // keep going while the branches are larger than min_length.

    Canopy canopy;
    struct QueueElement
    {
        Line2D line;
        int    depth = 0;
    };
    std::queue<QueueElement> queue;
    queue.push({root});

    // TODO: find a more elegant way
    while (!queue.empty()) {
        const auto current = queue.front();
        queue.pop();

        if (current.line.length() < min_length) {
            continue;    // go through every line in queue
        }
        canopy.push_back(current.line);

        if (current.depth > max_depth) {
            continue;
        }

        const auto [child_A, child_B] =
            gen_children(current.line, angle, scale_factor);
        queue.push({child_A, current.depth + 1});
        queue.push({child_B, current.depth + 1});
    }

    return canopy;
}

}    // namespace

int main()
{
    constexpr math::Degree child_rotation {60};
    constexpr auto         scale_factor = 0.5;
    constexpr auto         min_length   = 1;
    constexpr auto         max_depth    = 10;

    constexpr int          image_width  = 1'920;
    constexpr int          image_height = 1'080;

    Image                  image {image_width, image_height};

    constexpr Line2D       base {
              .start = {image_width / 2,     image_height},
              .end   = {image_width / 2, image_height / 2},
    };

    for (const auto &line : gen_canopy(base,
                                       child_rotation,
                                       scale_factor,
                                       max_depth,
                                       min_length)) {
        draw_line(image, line);
    }

    std::ofstream file {"FractalCanopy.ppm"};
    image.render(file);
}