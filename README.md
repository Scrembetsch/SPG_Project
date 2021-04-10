# SPG Project

This project is for my university course "Shader Programming"

## How to start

Open **spg.exe** in root directory of project

> The builded .exe in Release/Debug folder won't find the needed resoures

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
|    Key    | Function      |
| :-------: | ------------- |
|    ESC    | Close Window  |
|     W     | Move forward  |
|     A     | Move left     |
|     S     | Move backward |
|     D     | Move right    |
|   SPACE   | Move up       |
| LEFT_CTRL | Move down     |
|   MOUSE   | Look around   |

### Parallax Mapping
|  Key   | Function                  |
| :----: | ------------------------- |
|   1    | Decrease Parallax Scale   |
|   2    | Increase Parallax Scale   |
| &larr; | Decrease Steps            |
| &rarr; | Increase Steps            |
| &uarr; | Increase Refinement Steps |
| &darr; | Decrease Refinement Steps |

> Hold **L_SHIFT** while Increasing/Decreasing steps for faster changes

### Dolly Camera
|    Key    | Function                                      |
| :-------: | --------------------------------------------- |
| NUMPAD +  | Increase camera speed                         |
| NUMPAD -  | Decrease camera speed                         |
|     E     | Toggle Editor / Cinema mode                   |
| BACKSPACE | Delete all points in dolly list               |
|   ENTER   | Add current position & rotation to dolly list |
