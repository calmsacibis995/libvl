add_executable(libvl-test test.c)

target_include_directories(libvl-test PRIVATE ../src)
target_link_libraries(libvl-test vl)
