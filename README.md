# cse125-sp22-group4

## Build

- Package Manager: [Conan](https://conan.io/center/)

- Build System: [CMake](https://cmake.org/)

```bash
mkdir build
cd build
conan install .. -s build_type=Release
cmake ..
```