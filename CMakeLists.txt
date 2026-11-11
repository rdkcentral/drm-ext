cmake_minimum_required(VERSION 3.10)
project(drm-platform VERSION 0.2.0 LANGUAGES CXX)

include(GNUInstallDirs)
find_package(Threads REQUIRED)

set(DRM_PLATFORM_SOC_LIBRARY_PATH
    "${CMAKE_INSTALL_FULL_LIBDIR}/libDRMPlatformSoC.so"
    CACHE FILEPATH "Well-known SoC library path")
option(DRM_PLATFORM_BUILD_EXAMPLE_PLUGIN "Build the example SoC library" OFF)
option(DRM_PLATFORM_BUILD_TESTS "Build loader and dispatch tests" OFF)

add_library(drm-platform SHARED src/DRMPlatform.cpp)
add_library(DRMPlatform::drm-platform ALIAS drm-platform)
target_compile_features(drm-platform PUBLIC cxx_std_14)
target_include_directories(drm-platform PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_compile_definitions(drm-platform PRIVATE
    DRM_PLATFORM_SOC_LIBRARY_PATH="${DRM_PLATFORM_SOC_LIBRARY_PATH}")
target_link_libraries(drm-platform PRIVATE ${CMAKE_DL_LIBS} Threads::Threads)
set_target_properties(drm-platform PROPERTIES
    OUTPUT_NAME DRMPlatform
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    VERSION ${PROJECT_VERSION}
    SOVERSION 0)

if(DRM_PLATFORM_BUILD_EXAMPLE_PLUGIN)
    add_library(drm-platform-example MODULE examples/ExamplePlugin.cpp)
    target_compile_features(drm-platform-example PRIVATE cxx_std_14)
    target_include_directories(drm-platform-example PRIVATE include)
    set_target_properties(drm-platform-example PROPERTIES
        OUTPUT_NAME DRMPlatformSoC CXX_VISIBILITY_PRESET hidden)
endif()

if(DRM_PLATFORM_BUILD_TESTS)
    enable_testing()

    # Each process has its own singleton. Compile the same loader against an
    # explicit test path without adding a runtime path option to the public API.
    function(add_platform_test name library_path expected)
        add_executable(${name} tests/DRMPlatformTests.cpp src/DRMPlatform.cpp)
        target_compile_features(${name} PRIVATE cxx_std_14)
        target_include_directories(${name} PRIVATE include)
        target_compile_definitions(${name} PRIVATE
            DRM_PLATFORM_SOC_LIBRARY_PATH="${library_path}")
        target_link_libraries(${name} PRIVATE ${CMAKE_DL_LIBS} Threads::Threads)
        add_test(NAME ${name} COMMAND ${name} ${expected})
    endfunction()

    add_platform_test(drm-platform-missing
        "${CMAKE_CURRENT_BINARY_DIR}/missing/libDRMPlatformSoC.so" defaults)

    if(DRM_PLATFORM_BUILD_EXAMPLE_PLUGIN)
        add_platform_test(drm-platform-dispatch
            "$<TARGET_FILE:drm-platform-example>" soc)
        add_dependencies(drm-platform-dispatch drm-platform-example)

        add_library(drm-platform-rejected MODULE examples/ExamplePlugin.cpp)
        target_compile_features(drm-platform-rejected PRIVATE cxx_std_14)
        target_include_directories(drm-platform-rejected PRIVATE include)
        target_compile_definitions(drm-platform-rejected PRIVATE
            DRM_PLATFORM_TEST_REJECT_REGISTRATION)
        add_platform_test(drm-platform-rejection
            "$<TARGET_FILE:drm-platform-rejected>" defaults)
        add_dependencies(drm-platform-rejection drm-platform-rejected)
    endif()
endif()

install(TARGETS drm-platform EXPORT DRMPlatformTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY include/drm_platform DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT DRMPlatformTargets FILE DRMPlatformTargets.cmake
    NAMESPACE DRMPlatform::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/DRMPlatform)
