#include <iostream>
#include <string>

#include <cacs305/image.hpp>

namespace cacs305 {

void Image::render(std::ostream &out)
{
    out << "P3\n";
    out << _width << ' ' << _height << '\n';
    out << "255\n";
    std::string pixel_buf;
    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            pixel_buf += buf[y * _width + x].to_string() + '\n';
        }
        pixel_buf += '\n';
    }

    out << pixel_buf;
}

}    // namespace cacs305