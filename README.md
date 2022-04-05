# cse125-sp22-group4

## Build

- Package Manager: [Conan](https://conan.io/center/)

- Build System: [CMake](https://cmake.org/)

```bash
mkdir build
cd build
conan install .. -s build_type=Release --build missing
cmake ..
```

## If Using Visual Studio

Right click on solution Client, select "Set as Startup Project"

Go to Properties of solution Client, go to Debug, change working directory to `$(ProjectDir)/../..`

On the top menu, Change compile option to `Release` and `x64`

