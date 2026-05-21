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
    src/entity/UnitCatalogJson.cpp
    src/entity/LengthConversionEngine.cpp
    src/entity/DisplayRounder.cpp
    src/entity/UnitConverter.cpp
)
target_include_directories(unit_entity PUBLIC ${CMAKE_SOURCE_DIR}/src)

add_library(unit_data
    src/data/ConfigLoader.cpp
)
target_include_directories(unit_data PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(unit_data PUBLIC unit_entity)

add_library(unit_boundary
    src/boundary/InputParser.cpp
    src/boundary/OutputFormatter.cpp
)
target_include_directories(unit_boundary PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(unit_boundary PUBLIC unit_entity)

add_library(unit_control
    src/control/ConversionUseCase.cpp
)
target_include_directories(unit_control PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(unit_control PUBLIC unit_entity unit_boundary)

enable_testing()

function(add_unit_test target_name source_file)
    add_executable(${target_name} ${source_file})
    target_link_libraries(${target_name} PRIVATE unit_entity unit_data unit_boundary unit_control Catch2::Catch2WithMain)
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
target_link_libraries(unit_converter PRIVATE unit_entity unit_data unit_boundary unit_control)

add_executable(UnitConverter_test tests/test_golden_master.cpp)
target_link_libraries(UnitConverter_test PRIVATE Catch2::Catch2WithMain)
target_include_directories(UnitConverter_test PRIVATE ${CMAKE_SOURCE_DIR}/tests)
add_dependencies(UnitConverter_test unit_converter)
target_compile_definitions(UnitConverter_test PRIVATE
    UNIT_CONVERTER_EXE_PATH="$<TARGET_FILE:unit_converter>")
add_test(NAME GoldenMaster COMMAND UnitConverter_test
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
set_tests_properties(GoldenMaster PROPERTIES
    ENVIRONMENT "UNIT_CONVERTER_EXE=$<TARGET_FILE:unit_converter>")
