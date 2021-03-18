# SPG Project

This project is for my university course "Shader Programming"

## How to start

Open **ezg_algo.exe** in root directory of project

> The builded .exe in Release/Debug folder won't find the needed resoures

If a .obj file from the data/model folder should be loaded start the program with:
```sh
ezg_algo.exe <filename>
```

> The model will be loaded 10 times, as it is intended as a stress test for the kd-tree

## How to compile
Open Solution and adjust **Additional Include & Linker Directories**
> Default:
> * Include: **D:/OpenGL/include**
> * Linker: **D:/OpenGL/lib-vs2019**

### Dependencies
* GLFW: **3.3.2**
* GLM: **0.9.9.7**
* GLAD:
    * Language: **C/C++**
    * Specification: **OpenGL**
    * API:
        * gl: **Version 4.6**
        * Rest: **none**
    * Profile: **Core**
* Freetype: **2.10.4**
> Newer or older versions might work -> Not tested

## Controls

### General
|  Key  | Function      |
| :---: | ------------- |
|  ESC  | Close Window  |
|   W   | Move forward  |
|   A   | Move left     |
|   S   | Move backward |
|   D   | Move right    |
| MOUSE | Look around   |

### Dolly Camera
|    Key    | Function                                      |
| :-------: | --------------------------------------------- |
| NUMPAD +  | Increase camera speed                         |
| NUMPAD -  | Decrease camera speed                         |
|     E     | Toggle Editor / Cinema mode                   |
| BACKSPACE | Delete all points in dolly list               |
|   ENTER   | Add current position & rotation to dolly list |

### Normal map
|      Key      | Function                 |
| :-----------: | ------------------------ |
| Arrow &#8593; | Increase normal strength |
| Arrow &#8595; | Decrease normal strength |

### Anti-Aliasing
|  Key  | Function               |
| :---: | ---------------------- |
|   M   | Toggle MSAA on/off     |
|   1   | Set MSAA to GL_FASTEST |
|   2   | Set MSAA to GL_NICEST  |

### K-D Tree
|  Key  | Function                               |
| :---: | -------------------------------------- |
|   B   | Toggle bounding boxes on/off (drawing) |
|   V   | Toggle objects on/off (drawing)        |
|   R   | Shoot raycast in center of screen      |