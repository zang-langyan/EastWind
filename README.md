# 东风引擎 - East Wind Engine

## Build EastWind 
### MacOS
One option is simply execute the `rebuild.sh` shell script. if you have already generated the project, you can execute the `run.sh` script.

Another option is to generate and build the project manually. 
```shell
cmake -B build
cmake --build build
```

### Windows
#### Microsoft Visual Studio
**IMPORTANT: Intel OneApi Toolkit (MKL) needs to be installed**
```shell
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DMKL_INTERFACE_FULL=intel_lp64 --no-warn-unused-cli -B build -G "Visual Studio 17 2022" -T host=x64 -A x64
cmake --build build
```

#### MinGW
```shell
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -B build -G "Unix Makefiles"
cmake --build build
```

## Draft Structure
1. [x] Entry Point
2. [x] Application Layer
3. [x] Window Layer
    - [x] Inputs
    - [x] Events
4. [x] Renderer
5. [x] Render API Abstractions
6. [x] Debugging Support
7. [x] Hot Reloading Framework
8. [ ] Scripting Language Support
9. [ ] Memory System
10. [ ] Entity-Component System
11. [ ] Physics
12. [ ] File I/O, Virtual File System
13. [ ] Build System -> Data Format Support
14. [ ] Audio System
15. [ ] (G)UI System

## Feature Support
1. [ ] Multi-Thread Support
2. [ ] Platform Specific Support
    - [ ] OS Specific
        - [x] Windows
        - [x] MacOS
        - [ ] Linux
    - [ ] Graphics API Specific
        - [x] OpenGL
        - [ ] Vulkan
        - [ ] DirectX 12
        - [ ] Metal
