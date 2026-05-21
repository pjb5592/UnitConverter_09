cmake_minimum_required(VERSION 3.16)
project(UnitConverter VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.4
)
FetchContent_MakeAvailable(Catch2)

add_library(unit_entity
    src/entity/UnitDefinition.cpp
    src/entity/UnitCatalog.cpp
    src/entity/LengthConversionEngine.cpp
    src/entity/DisplayRounder.cpp
)
target_include_directories(unit_entity PUBLIC ${CMAKE_SOURCE_DIR}/src)

add_library(unit_data
    src/data/ConfigLoader.cpp
)
target_include_directories(unit_data PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(unit_data PUBLIC unit_entity)

add_library(unit_boundary
    src/boundary/InputParser.cpp
)
target_include_directories(unit_boundary PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(unit_boundary PUBLIC unit_entity)

enable_testing()

function(add_unit_test target_name source_file)
    add_executable(${target_name} ${source_file})
    target_link_libraries(${target_name} PRIVATE unit_entity unit_data unit_boundary Catch2::Catch2WithMain)
    target_include_directories(${target_name} PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/tests)
    add_test(NAME ${target_name} COMMAND ${target_name})
endfunction()

add_unit_test(unit_conversion_tests tests/unit/entity/conversion_tests.cpp)
add_unit_test(unit_boundary_tests tests/unit/boundary/parser_tests.cpp)
add_unit_test(unit_catalog_tests tests/unit/entity/catalog_registration_tests.cpp)
add_unit_test(unit_config_tests tests/unit/data/config_loader_tests.cpp)
add_unit_test(unit_logic_red_tests tests/unit/entity/logic_track_red_tests.cpp)
add_unit_test(unit_ui_red_tests tests/unit/boundary/ui_track_red_tests.cpp)

add_executable(unit_converter src/main.cpp)
target_link_libraries(unit_converter PRIVATE unit_entity unit_data unit_boundary)
