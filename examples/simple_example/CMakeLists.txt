
# Configure example excutable.
find_package(csys REQUIRED)
add_executable(simple_example simple_example_main.cpp)

# Link with csys library.
target_link_libraries(simple_example csys::csys)