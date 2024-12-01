# PathTracer

## Build With CMake

### MinGW

git-bash

```bash
mkdir build && cd build
cmake .. -G"MinGW Makefiles" -DCMAKE_BUILD_TYPE="Release"
mingw32-make -j 30
```

powershell

```shell
New-Item -Path build -ItemType Directory; Set-Location build
cmake.exe .. -G"MinGW Makefiles" -DCMAKE_BUILD_TYPE="Release"
mingw32-make.exe -j 30
```

### MSVC

#### Using Visual Studio With CMake

If Your Visual Studio Installed With CMake Component, Just Open The Project In Visual Studio.

#### Using Visual Studio Without CMake

git-bash

```bash
mkdir build && cd build
cmake ..
```

powershell

```shell
New-Item -Path build -ItemType Directory; Set-Location build
cmake.exe ..
```

After Generate, Double-Click The Solution File With Extension `.sln` And Build The Target `ALL_BUILD`
