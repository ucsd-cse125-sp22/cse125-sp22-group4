# cse125-sp22-group4
## Build
- Build System: [CMake](https://cmake.org/)
>>>>>>> origin/graphics

```bash
mkdir build
cd build
<<<<<<< HEAD
cmake ..
```

- Inside build directory, there would be `cse125-sp22-group4.sln`.

- Inside visual studio, right click on the solution to set startup projects.

- Select `Server` and `Client` as startup projects, and place `Server` before `Client`.
=======
conan install .. -s build_type=Release --build missing
cmake ..
```

## If Using Visual Studio

- Right click on solution Client, select "Set as Startup Project".

- Go to Properties of solution Client, go to Debugging, change working directory to `$(ProjectDir)/../..`

- On the top menu, change compile option to `Release` and `x64`.


>>>>>>> origin/graphics
