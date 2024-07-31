add_library(image-framework 
    image.cpp
)

target_include_directories(image-framework PUBLIC "${CMAKE_SOURCE_DIR}/include")
