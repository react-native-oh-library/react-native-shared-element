cmake_minimum_required(VERSION 3.13)
set(CMAKE_VERBOSE_MAKEFILE on)

set(rnoh_shared_element_generated_dir "${RNOH_GENERATED_DIR}/react_native_openharmony_shared_element")
file(GLOB_RECURSE rnoh_shared_element_generated_SRC "${rnoh_shared_element_generated_dir}/**/*.cpp")
file(GLOB rnoh_shared_element_SRC CONFIGURE_DEPENDS *.cpp helper/*.cpp view/*.cpp)
add_library(rnoh_shared_element SHARED ${rnoh_shared_element_SRC} ${rnoh_shared_element_generated_SRC})
target_include_directories(rnoh_shared_element PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${rnoh_shared_element_generated_dir})
target_link_libraries(rnoh_shared_element PUBLIC rnoh)
