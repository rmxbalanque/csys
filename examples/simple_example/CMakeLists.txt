
# Configure example excutable.
add_executable(simple_example simple_example_main.cpp)

# Link with ccli library.
target_link_libraries(simple_example ccli::ccli)