# Sssnake
![example workflow](https://github.com/MRickers/SSsnake/blob/master/.github/workflows/cmake.yml/badge.svg)

## Build

### Linux

#### ubuntu
Install SDL2 dependencies
```
sudo apt-get update
sudo apt-get install -y libsdl2-dev
```

#### Build project
Clone dependencies
```
git submodule init
git submodule update --recursive
```
Build
```
mkdir build
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug
```

## Tests
Run tests after build
```
cd build
ctest -C Debug
```
