#include <cstdlib>
#include <cacs305/image.hpp>
#include <fstream>

using namespace cacs305;

Color::byte random_byte()
{
    return Color::byte(1 + rand() / ((RAND_MAX + 1u) / 255));
}

Color random_color()
{
    return Color {.red   = random_byte(),
                  .green = random_byte(),
                  .blue  = random_byte()};
}

int main()
{
    const int image_width  = 1'920;
    const int image_height = 1'080;
    Image     image {image_width, image_height};

    for (int x = 0; x < image_width; ++x) {
        for (int y = 0; y < image_height; ++y) {
            image.put_pixel(x, y, random_color());
        }
    }

    std::ofstream file("random_noise.ppm");
    image.render(file);
}