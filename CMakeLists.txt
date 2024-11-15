CMAKE_MINIMUM_REQUIRED(VERSION 3.29)

PROJECT(PathTracer
  VERSION 1.0.0
  DESCRIPTION "CG Homework 2024 Winter"
  HOMEPAGE_URL "https://github.com/AliceRemake/PathTracer"
  LANGUAGES C CXX
)

SET(CMAKE_CXX_STANDARD 23 REQUIRED)

ADD_SUBDIRECTORY(External)

ADD_EXECUTABLE(PathTracer)

IF(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  TARGET_COMPILE_DEFINITIONS(PathTracer PUBLIC CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR})
ELSE()
  TARGET_COMPILE_DEFINITIONS(PathTracer PUBLIC NDEBUG)
ENDIF()

IF(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  TARGET_COMPILE_OPTIONS(PathTracer PUBLIC
    "-fno-exceptions" "-fno-rtti" "-Wall" "-Wextra" "-Werror"
  )
ELSE()
  TARGET_COMPILE_OPTIONS(PathTracer PUBLIC
    "-O3" "-fno-exceptions" "-fno-rtti" "-w"
  )
ENDIF()

TARGET_INCLUDE_DIRECTORIES(PathTracer PUBLIC
  ${CMAKE_SOURCE_DIR}
)

TARGET_SOURCES(PathTracer PUBLIC
  ${CMAKE_SOURCE_DIR}/main.cpp
  ${CMAKE_SOURCE_DIR}/Core/Common.h
  ${CMAKE_SOURCE_DIR}/Core/Debug.h
  ${CMAKE_SOURCE_DIR}/Core/Debug.cpp
  ${CMAKE_SOURCE_DIR}/Core/Camera.h
  ${CMAKE_SOURCE_DIR}/Core/Camera.cpp
  ${CMAKE_SOURCE_DIR}/Core/Scene.h
  ${CMAKE_SOURCE_DIR}/Core/Scene.cpp
)

TARGET_LINK_LIBRARIES(PathTracer PUBLIC
  fmt::fmt
  stb::stb
  tinyxml2::tinyxml2
  Eigen3::Eigen
)
