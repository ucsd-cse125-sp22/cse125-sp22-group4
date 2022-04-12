# cse125-sp22-group4

## Build

- Package Manager: [Conan](https://conan.io/)
- Build System: [CMake](https://cmake.org/)

```bash
mkdir build
cd build
conan install .. -s build_type=Release --build missing
cmake ..
```

## Visual Studio Setup

- Inside build directory, there would be `cse125-sp22-group4.sln`.
- Inside visual studio, right click on the solution to set startup projects.
- Select `Server` and `Client` as startup projects, and place `Server` before `Client`.
- Go to Properties of target `Client`, go to Debugging, change working directory to `$(ProjectDir)/../..`
- On the top menu, change compile option to `Release` and `x64`.
