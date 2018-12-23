# GTi-Engine
3D Game Engine in develompent by Marc Fabian & Albert Llopart, students at CITM-UPC Terrassa.

- Engine (https://github.com/albertllopart/GTi-Engine-Reborn) @github repository
- Marc Fabian (https://github.com/xDragan) @github
- Albert Llopart (https://github.com/albertllopart) @github

This engine uses C++ language.

## Systems

### Hierarchy

The hierarchy contains all Game Objects in the scene ordered with parents and children. The main parent, or the Root, we call it Scene 1 by default and it will contain every Game Object created as a children, and at the same time every Game Object will be able to have the same structure at a smaller scale.

If there were any changes in a Component Transform of a Game Object all of its children would be affected by it but not vice-versa.

You can add or remove Game Objects using the proper buttons lower in the Hierarchy window.

<img src="Web Images/jerarquia.PNG" alt="hi" class="inline"/>

### Inspector

The inspector shows information about the currently selected Game Object. There will always be a Component Transform showing position, rotation and scale.

<img src="Web Images/transform.PNG" alt="hi" class="inline"/>

You can add a Component Mesh (which will contain information about the mesh you have loaded into that particular Game Object <Name of the source, Number of vertices, Number of indices>). 

<img src="Web Images/mesh.PNG" alt="hi" class="inline"/>

Component Material (which will contain information about the texture loaded in memory for that GameObject and also contains a menu to edit and apply Shaders to the Component Mesh in that very Game Object).

<img src="Web Images/material.PNG" alt="hi" class="inline"/>

Component Camera (which will allow you to set a camera following the Component Transform indications and you will also be able to adjust its frustum parameters).

<img src="Web Images/camera.PNG" alt="hi" class="inline"/>

### Rescource Manager

Our Resource Manager is limited to allow us to load meshes and textures from the Engine itself. We can also drag and drop them on it. The Resource Manager checks for the contents of Assets and shows them at the lowest window in the engine. There you can navidate through its files and click Load button when you want to load them in a Game Object.

<img src="Web Images/resource_manager.PNG" alt="hi" class="inline"/>

### Shader Pipeline

Our drawing system is all made using shaders. 

If a mesh is loaded with no Material a default shader will be used to draw each triangle (white). If you add a component Material to the Game Object that contains the mesh you can choose to either use a Sample Shader (which consists in a cartoonish water inspired by The Legend Of Zelda: The Wind Waker) or to use your own shader. 

If you want to use your own shader you can edit both Vertex and Fragment documents by using the inspector. Simply select the type of shader object you wish to edit (Vertex or Fragment) and press INTRO. A text editor will pop up with a default shader GLSL code. (If you prefer a blank document just click the Open Editor button). Once you have saved and compiled both a Vertex and a Fragment Shader you can click on "Create Shader Program" to bind them into a program and start using it. 

<img src="Web Images/shader_pipeline_1.PNG" alt="hi" class="inline"/>

If you wish to edit a certain object which already exists in Library simply write the name of the file and the Text editor will contain your previously saved GLSL code.

### Module Configuration

- Application: Here you can check Framerate or even adjust it. Also you can change the name of the App or Organization.
- Window: Here you can set window brightness, width and height, as well as fullscreen, borderless or full desktop.
- Camera: Here you can adjust camera frustom parameters and you can also decide to render the raycast that the engine uses in order to do the mouse picking.
- Renderer: Here you can enable or disable many OpenGL rendering options such as Wireframe mode, Depth test, Cull face, Lighting, Color Material, 2D Textures, Show Normals or Show Quadtree.
- Textures: Here you can check the textures that you have already loaded in memory.
- Hardware: This section contains information about PC hardware and also some information about SDL, DeviL and GLSL versions.

<img src="Web Images/options.PNG" alt="hi" class="inline"/>

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


