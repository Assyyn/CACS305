link_libraries(image-framework)

function(setup_example source_name)
    message("CACS305: ADDING EXAMPLE ${source_name}...")
    add_executable(${source_name} ${source_name}.cpp)
endfunction()

setup_example(DDA)
setup_example(FloodFill4way)
setup_example(Bresenham)