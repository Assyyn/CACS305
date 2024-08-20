#include <iostream>

#include <cacs305/image.hpp>

namespace cacs305 {

void Image::render(std::ostream &out)
{
    out << render();
}

}    // namespace cacs305