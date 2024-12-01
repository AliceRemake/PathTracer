# PathTracer

A Monte Carlo Path Tracer.

## Main Reference

Ray Tracing in One Weekend - The Book Series

[[github](https://github.com/RayTracing/raytracing.github.io)][[website](https://raytracing.github.io/)]

## Gallery

See Render Results Here: [Gallery](Doc/README.md)

## Third Party Libraries

* [fmt](https://github.com/fmtlib/fmt): A modern formatting library.
* [stb](https://github.com/nothings/stb): stb single-file public domain libraries for C/C++.
* [tinyxml2](https://github.com/leethomason/tinyxml2): TinyXML2 is a simple, small, efficient, C++ XML parser that can be easily integrated into other programs.
* [tinyobjloader](https://github.com/tinyobjloader/tinyobjloader): Tiny but powerful single file wavefront obj loader
* [eigen](https://gitlab.com/libeigen/eigen): Eigen is a C++ template library for linear algebra: matrices, vectors, numerical solvers, and related algorithms.

## Software Architecture

### Conventions

1. All Direction/Normal Vectors Are Unit Vectors. No Need To Normalize Them Before Using But Need To Normalize Them After Creating.

## Build With CMake

Clone The Repository With Submodules.

```bash
git clone --recurse-submodules -j8 https://github.com/AliceRemake/PathTracer
```

### MinGW

```bash
mkdir build && cd build
cmake .. -G"MinGW Makefiles" -DCMAKE_BUILD_TYPE="Release"
mingw32-make -j30
```

### MSVC

#### Using Visual Studio With CMake

If Your Visual Studio Installed With CMake Component, Just Open The Project In Visual Studio.

#### Using Visual Studio Without CMake

```bash
mkdir build && cd build
cmake ..
```

After Generate, Double-Click The Solution File With Extension `.sln` And Build The Target `ALL_BUILD`
