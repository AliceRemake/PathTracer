CMAKE_MINIMUM_REQUIRED(VERSION 3.29)

PROJECT(PathTracer
    VERSION 1.0.0
    DESCRIPTION "CG Homework 2024 Winter"
    HOMEPAGE_URL "https://github.com/AliceRemake/PathTracer"
    LANGUAGES C CXX
)

IF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt. ")
ENDIF()

SET(CMAKE_OBJECT_PATH_MAX 512)

SET(CMAKE_CXX_STANDARD 20 REQUIRED)

IF(NOT DEFINED CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "Release")
ENDIF()

# Third Party Libraries.
ADD_SUBDIRECTORY(External)

# Unit Tests.
IF(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    ADD_SUBDIRECTORY(Test)
ENDIF()

ADD_EXECUTABLE(PathTracer)

IF(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    TARGET_COMPILE_DEFINITIONS(PathTracer PRIVATE CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR})
ELSE()
    TARGET_COMPILE_DEFINITIONS(PathTracer PRIVATE CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR} NDEBUG ENABLE_MULTI_THREAD)
ENDIF()

MESSAGE(STATUS "CMAKE_C_COMPILER_ID: ${CMAKE_C_COMPILER_ID}")
MESSAGE(STATUS "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")

IF(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
    IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
        TARGET_COMPILE_OPTIONS(PathTracer PRIVATE
            "-fno-exceptions" "-fno-rtti" "-Wall" "-Wextra" "-Werror" "-march=native"
        )
    ELSE()
        TARGET_COMPILE_OPTIONS(PathTracer PRIVATE
            "-O3" "-fno-exceptions" "-fno-rtti" "-Wall" "-Wextra" "-Werror" "-march=native"
        )
    ENDIF()
ELSEIF(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
    IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
        TARGET_COMPILE_OPTIONS(PathTracer PRIVATE
            "/D_HAS_EXCEPTIONS=0" "/GR-" "/W4" "/WX" "/Zc:__cplusplus"
        )
    ELSE()
        TARGET_COMPILE_OPTIONS(PathTracer PRIVATE
            "/Ot" "/D_HAS_EXCEPTIONS=0" "/GR-" "/W4" "/WX" "/Zc:__cplusplus"
        )
    ENDIF()
ELSE()
    MESSAGE(FATAL_ERROR "Unsupported CXX Compiler: ${CMAKE_CXX_COMPILER_ID}")
ENDIF()

TARGET_INCLUDE_DIRECTORIES(PathTracer PRIVATE
    ${CMAKE_SOURCE_DIR}
)

TARGET_SOURCES(PathTracer PRIVATE
    ${CMAKE_SOURCE_DIR}/main.cpp
    ${CMAKE_SOURCE_DIR}/cpp.hint
    ${CMAKE_SOURCE_DIR}/Core/Common.h
    ${CMAKE_SOURCE_DIR}/Core/Debug.h
    ${CMAKE_SOURCE_DIR}/Core/Debug.cpp
    ${CMAKE_SOURCE_DIR}/Core/RNG.h
    ${CMAKE_SOURCE_DIR}/Core/RNG.cpp
    ${CMAKE_SOURCE_DIR}/Core/ThreadPool.h
    ${CMAKE_SOURCE_DIR}/Core/ThreadPool.cpp
    ${CMAKE_SOURCE_DIR}/Core/Parallel.h
    ${CMAKE_SOURCE_DIR}/Core/Parallel.cpp
    ${CMAKE_SOURCE_DIR}/Core/ONB.h
    ${CMAKE_SOURCE_DIR}/Core/ONB.cpp
    ${CMAKE_SOURCE_DIR}/Core/Interval.h
    ${CMAKE_SOURCE_DIR}/Core/Interval.cpp
    ${CMAKE_SOURCE_DIR}/Core/Image.h
    ${CMAKE_SOURCE_DIR}/Core/Image.cpp
    ${CMAKE_SOURCE_DIR}/Core/Sampler.h
    ${CMAKE_SOURCE_DIR}/Core/Sampler.cpp
    ${CMAKE_SOURCE_DIR}/Core/Material.h
    ${CMAKE_SOURCE_DIR}/Core/Material.cpp
    ${CMAKE_SOURCE_DIR}/Core/Texture.h
    ${CMAKE_SOURCE_DIR}/Core/Texture.cpp
    ${CMAKE_SOURCE_DIR}/Core/Hittable.h
    ${CMAKE_SOURCE_DIR}/Core/Hittable.cpp
    ${CMAKE_SOURCE_DIR}/Core/Primitive.h
    ${CMAKE_SOURCE_DIR}/Core/Primitive.cpp
    ${CMAKE_SOURCE_DIR}/Core/Mesh.h
    ${CMAKE_SOURCE_DIR}/Core/Mesh.cpp
    ${CMAKE_SOURCE_DIR}/Core/Ray.h
    ${CMAKE_SOURCE_DIR}/Core/Ray.cpp
    ${CMAKE_SOURCE_DIR}/Core/Camera.h
    ${CMAKE_SOURCE_DIR}/Core/Camera.cpp
    ${CMAKE_SOURCE_DIR}/Core/Renderer.h
    ${CMAKE_SOURCE_DIR}/Core/Renderer.cpp
    ${CMAKE_SOURCE_DIR}/Core/Bounds.h
    ${CMAKE_SOURCE_DIR}/Core/Bounds.cpp
)

TARGET_LINK_LIBRARIES(PathTracer PRIVATE
    fmt::fmt
    stb::stb
    tinyxml2::tinyxml2
    tinyobjloader::tinyobjloader
    Eigen3::Eigen
)
