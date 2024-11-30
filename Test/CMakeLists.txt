ADD_SUBDIRECTORY(External)

ADD_COMPILE_DEFINITIONS(
    DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
    DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
    CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR}
)

IF(CMAKE_CXX_COMPILER_FRONTEND_VARIANT MATCHES "GNU")
    IF(CMAKE_BUILD_TYPE MATCHES "Debug")
        ADD_COMPILE_OPTIONS(
          "-fno-exceptions" "-fno-rtti" "-Wall" "-Wextra" "-Werror" "-march=native"
        )
    ELSE()
        ADD_COMPILE_OPTIONS(
          "-O3" "-fno-exceptions" "-fno-rtti" "-Wall" "-Wextra" "-Werror" "-march=native"
        )
    ENDIF()
ELSEIF(CMAKE_CXX_COMPILER_FRONTEND_VARIANT MATCHES "MSVC")
    IF(CMAKE_BUILD_TYPE MATCHES "Debug")
        ADD_COMPILE_OPTIONS(
            "/D_HAS_EXCEPTIONS=0" "/GR-" "/W4" "/GR-" "/W4"
        )
    ELSE()
        ADD_COMPILE_OPTIONS(
            "/Ot" "/D_HAS_EXCEPTIONS=0" "/GR-" "/W4" "/GR-" "/W4"
        )
    ENDIF()
ELSE()
    MESSAGE(FATAL_ERROR "Unsupported CXX Compiler: ${CMAKE_CXX_COMPILER_ID}")
ENDIF()

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

LINK_LIBRARIES(
    fmt::fmt
    stb::stb
    tinyxml2::tinyxml2
    Eigen3::Eigen
    doctest::doctest
)

ADD_EXECUTABLE(
    TestCommon
    TestCommon.cpp
    ${CMAKE_SOURCE_DIR}/Core/Common.h
)

ADD_EXECUTABLE(
    TestInterval
    TestInterval.cpp
    ${CMAKE_SOURCE_DIR}/Core/Common.h
    ${CMAKE_SOURCE_DIR}/Core/Interval.h
    ${CMAKE_SOURCE_DIR}/Core/Interval.cpp
)

ADD_EXECUTABLE(
    TestThreadPool
    TestThreadPool.cpp
    ${CMAKE_SOURCE_DIR}/Core/ThreadPool.h
    ${CMAKE_SOURCE_DIR}/Core/ThreadPool.cpp
)

ADD_EXECUTABLE(
    TestParallel
    TestParallel.cpp
    ${CMAKE_SOURCE_DIR}/Core/Parallel.h
    ${CMAKE_SOURCE_DIR}/Core/Parallel.cpp
)

ADD_EXECUTABLE(
    TestImage
    TestImage.cpp
    ${CMAKE_SOURCE_DIR}/Core/Image.h
    ${CMAKE_SOURCE_DIR}/Core/Image.cpp
)

ADD_EXECUTABLE(
    TestBounds
    TestBounds.cpp
    ${CMAKE_SOURCE_DIR}/Core/Bounds.h
    ${CMAKE_SOURCE_DIR}/Core/Bounds.cpp
)
