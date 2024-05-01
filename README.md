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
**The important part is to have the GNU compilers g++ and gfortran installed. This project does not support the Microsoft ifort and VS C++ compiler for now.**
```shell
cmake -B build -G "Unix Makefiles"
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
7. [ ] Scripting Lang Support
8. [ ] Memory System
9. [ ] Entity-Component System
10. [ ] Physics
11. [ ] File I/O, Virtual File System
12. [ ] Build System -> Data Format Support
13. [ ] Audio System
14. [ ] (G)UI System

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
