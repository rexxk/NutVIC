# NutVIC/src

target_sources(NutVIC PRIVATE NutVIC.cpp)


add_subdirectory(chipset)
add_subdirectory(cpu)
add_subdirectory(memory)
add_subdirectory(ui)
add_subdirectory(utils)
