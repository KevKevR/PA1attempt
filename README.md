Programming Assignment 1 
======
for COMP371 CC, Summer 2020
with instructor Serguei A. Mokhov, 
due by the end of July 12th, 2020.

Team List
======
- Kevin Rao (40095427)
- Manuel Romero (40023706)
- Gia-Khang Ernest Nguyen (40091426)
- Wing-Fei Jason Tiu (40048495)
- Jason Beccherini (40130107)

Features and Functionality
======
//TODO
(OpenGL, grid, axes, models, positioning, single cube object, colored, movable camera and models, camera focus on origin. Maybe more.)


Display
----
Program only supports the following display option
- Window size of 1024 * 768
- 4:3 aspect ratio
- Double buffering 

Controls
======
Note that there is a distinction between 'a' and 'A'. Capital letters require holding the shift key (left).

Model Controls
------
- Number keys 1 through 5 to select one of the five models. 
The following controls apply to the selected models only.
- WASD to move model in XY plane.
- a and d to rotate model about y-axis.
- Home key to reset model position and orientation to initial settings. This will also reset the camera's.

Camera Controls
------
- Arrow keys to pitch and yaw camera.
- Home key to reset camera to initial settings. This will also reset the selected model's.
- While holding the left mouse button, vertical mouse motions will zoom the camera in and out (z-axis movement).
- While holding the middle mouse button, horizontal mouse motions will pan the camera from side to side(rotation about y-axis).
- While holding the right mouse button, vertical mouse motions will tilt(pitch) the camera up and down(rotation about x-axis).

Rendering Controls
------
The models will be rendered according to the following scheme:
- T    Triangles (Fill/Surfaces)
- L    Lines (wireframe)
- P    Points (Vertices)
