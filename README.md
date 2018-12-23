# GTi-Engine
3D Game Engine in develompent by Marc Fabian & Albert Llopart, students at CITM-UPC Terrassa.

- Engine (https://github.com/albertllopart/GTi-Engine-Reborn) @github repository
- Marc Fabian (https://github.com/xDragan) @github
- Albert Llopart (https://github.com/albertllopart) @github

This engine uses C++ language.

## Systems

### Hierarchy

### Inspector

### Rescource Manager

### Shader Pipeline

Our drawing system is all made using shaders. 

If a mesh is loaded with no Material a default shader will be used to draw each triangle (white). If you add a component Material to the Game Object that contains the mesh you can choose to either use a Sample Shader (which consists in a cartoonish water inspired by The Legend Of Zelda: The Wind Waker) or to use your own shader. 

If you want to use your own shader you can edit both Vertex and Shader documents by using the inspector. Simply select the type of shader object you wish to edit (Vertex or Fragment) and select edit. A text editor will pop up with a default shader GLSL code. Once you have saved and compiled both a Vertex and a Fragment Shader you can click on "Create Shader Program" to bind them into a program and start using it. 

If you wish to edit a certain object which already exists in Library simply write the name of the file and the Text editor will contain your previously saved GLSL code.

### Module Options

## Camera

- RightMouse: Rotate
- RightMouse +:
	- W: move forward
	- A: move left
	- S: move backwards
	- D: move right
- WheelMouse:
	- FW: Zoom in
	- BW: Zoon out
- Alt + LeftMouse: Orbit mesh
- F: focus camera around geometry
- Holding shift duplicates speed*

## Config

- Alt + f4: exit program
- f5: save config
- f6: load config

## Libraries

- SDL 2.0.8
- MathGeoLib 1.3
- ImGui 1.52
- ImGuizmo 
- OpenGL 4.6
- Glew 2.1.0
- DevIl 178
- Assimp 3.3.0
- Parson
- PCG 0.94

## Guizmos
Using ImGuizmos for transformation helper. (https://github.com/CedricGuillemet/ImGuizmo)

Using W,E,R shorcuts transform position, rotate or scale.

## License

MIT License

Copyright (c) 2018 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


