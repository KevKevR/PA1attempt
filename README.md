Programming Assignment 2
======
for COMP371 CC, Concordia University Summer 2020
with instructor Serguei A. Mokhov, 
due by the end of July 27th, 2020.

Team List
======
- Jason Beccherini (40130107)
- Manuel Romero (40023706)
- Gia-Khang Ernest Nguyen (40091426)
- Wing-Fei Jason Tiu (40048495)
- Kevin Rao (40095427)

Features and Functionality
======
This program takes advantage of what OpenGL v3.1 and onwards offers to render colored 3D models with a movable perspective camera.
The ground is shown with a 100x100 unit grid centered at the origin and it is illuminated by a point light source placed 30 units above it. The entire scene is rendered in a tiled texture and shadows. Standard xyz axes are also centered at the origin, and are colored in rgb fashion. 
Models representing each of the students are arbitrarily placed in each of the four corners and center positions. They are models of the following letter-number pairs: S3, N2, A9, N4, V9. All models were drawn with copies of a same, single unit cube object, applied with various transformation to stretch and position them in shapes of alphanumeric characters. Each one is also paired with a single unit sphere object that covers the upper half of the models. The letters and numbers are rendered with textures, with the former having a box-lie texture and the latter shiny metal textures. These are free to move in the XZ-plane individually through the **models controls** section below. Additional modifications are possible and found in said section. Note that some of these modification apply once per key press, while others are applied continuously as long as the valid key combination is held.
The view camera initially centers its focus onto the origin point (0, 0, 0). Additional camera and display properties can be found in the **Display/Camera** section. The camera moves in first person view tradition. Further details on how to move it are found in the **Camera Controls** section.
Rendering mode can change between triangles, lines and points. Controls found in **Rendering Controls** section.


Display/Camera
----
Program only supports the following display option and camera setting
- Window size of 1024 * 768
- 4:3 aspect ratio
- Double buffering 
- Perspective projection
- Hidden surface removal with backface culling and depth test

Controls
======
Note that there is a distinction between 'a' and 'A'. Capital letters require holding the shift key (left or right).

Model Controls
------
- Number keys 1 through 5 to select one of the five models (assigned in this order: S3, N2, A9, N4, V9). 

The following controls apply to the *selected model* only.
- Spacebar to re-position the model at a random location on the grid.
- WASD to move model in XZ plane.
- a and d to incrementally rotate model about y-axis (5 degrees per press).
- U and J to incrementally scale model to be bigger or smaller.
- Home key to reset model position and orientation to initial settings. This will also reset the camera's.


Camera Controls
------
The camera view can be controlled as followed:
- Arrow keys to pitch and yaw camera. Holding shift will increase the speed.
- Use hbnm to move the camera's position in/out and side-to-side of the scene. Holding shift will increase the speed.
- Home key to reset camera to initial settings. This will also reset the selected model's.
- While holding the left mouse button, vertical mouse motions will zoom the camera in and out (z-axis movement).
- While holding the middle mouse button, horizontal mouse motions will pan the camera from side to side(rotation about y-axis).
- While holding the right mouse button, vertical mouse motions will tilt(pitch) the camera up and down(rotation about x-axis).

Rendering Controls
------
The models will be rendered according to the following scheme:
- T    Triangles (like Fill/Surfaces)
- L    Lines (like transparent wireframe)
- P    Points (Vertices)

The model and scenery textures and shadows can be turned on and off according to the following scheme:
- X    Textures
- B    Shadows
