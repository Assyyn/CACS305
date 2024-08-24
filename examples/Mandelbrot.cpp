#include "cacs305/utilities.hpp"
#include <complex>
#include <cacs305/image.hpp>
#include <fstream>
#include <string_view>

using namespace cacs305;
namespace {

struct Range
{
    float start {};
    float end {};

    constexpr static float
    transform(float value, const Range &from, const Range &to)
    {
        return to.start
             + (value - from.start) * (to.end - to.start)
                   / (from.end - from.start);
    }
};

namespace impl {
using byte = Color::byte;
static constexpr std::array<Color, 256> palette {[]() {
    std::array<Color, 256> palette;
    for (byte i = 0; i < 64; ++i) {
        palette[i] = (Color {.red   = 0,
                             .green = narrow_cast<byte>(i * 4),
                             .blue  = narrow_cast<byte>(255 - i * 4)});
    }
    for (byte i = 0; i < 64; ++i) {
        palette[i + 64] = (Color {.red   = 0,
                                  .green = narrow_cast<byte>(255 - i * 4),
                                  .blue  = narrow_cast<byte>(128 + i * 4)});
    }
    for (int i = 128; i < 256; ++i) {
        palette[i] = (Color {.red   = narrow_cast<byte>(255 - i),
                             .green = narrow_cast<byte>(255 - i),
                             .blue  = narrow_cast<byte>(255 - i)});
    }

    return palette;
}()};

}    // namespace impl

constexpr Color color_mapping(int iterations)
{
    using impl::palette;
    if (iterations >= palette.size()) {
        return palette.back();
    }

    return palette[iterations];
}

}    // namespace

int main()
{
    constexpr int              max_iterations = 1'000;
    constexpr std::string_view filename {"mandelbrot.ppm"};

    constexpr std::size_t      image_width  = 1'920;
    constexpr std::size_t      image_height = 1'080;

    Image                      image {image_width, image_height};

    constexpr Range            mbrot_r_range {-2.f, 0.47f};
    constexpr Range            mbrot_i_range {-1.12f, 1.12f};

    constexpr Range            image_x_range {0, image_width - 1};
    constexpr Range            image_y_range {0, image_height - 1};

    for (int y = 0; y < image_height; ++y) {
        for (int x = 0; x < image_width; ++x) {
            const std::complex<float> c {
                Range::transform(x, image_x_range, mbrot_r_range),
                Range::transform(y, image_y_range, mbrot_i_range)};

            int iterations = 0;
            {
                for (std::complex<float> z {};
                     std::abs(z) <= 2 && iterations < max_iterations;
                     z = z * z + c) {
                    ++iterations;
                }
            }
            image.put_pixel(x, y, color_mapping(iterations));
        }
    }

    std::ofstream mandelbrot(filename.data());
    image.render(mandelbrot);
}