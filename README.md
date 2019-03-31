# VLAD Heavy Strike

This is the full C++ source code to <a href="https://www.franticsoftware.com/vlad">VLAD Heavy Strike</a>, a mobile 3D shooter released in 2013.

In addition to the game source, this includes the full game pipeline (engine, editor, and tools).

<b>Non exhaustive list of Engine Features</b>

Low Level:

+ Cross platform targets (PC / iOS / Android / Windows Phone 8)<br>
+ 3D rendering using OpenGL, GLES1.1, and DirectX11<br>
+ Memory allocation tracking (full leak & allocation reporting)<br>
+ 3D Math library<br>
+ File system abstraction<br>
+ Network socket abstraction<br>
+ Multi-threading abstraction<br>
+ Touch and Key input abstraction<br>
+ FBX and PNG support through fbxsdk and libpng<br>
+ OGG and WAV support through cricket audio<br>
+ Event system<br>

High Level:

+ Object Model framework, with serialization, ref. counting, weak handles, and polymorphism support<br>
+ Entity Component System (ECS)<br>
+ Generic entity hierarchy handling allowing to have 3D or 2D hierarchies, or both in the same tree<br>
+ Quad-tree partitioning<br>
+ Collision handling<br>
+ Context stacking and switching<br>

Tool Side:
+ WYSIWYG Game Editor for Game and Level design<br>
+ The game can be played in the editor and game contexts can be inspected at run-time<br>
+ ObjectViewer tool, allows to inspect all alive objects at any given time<br>
