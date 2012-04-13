Requirements
============

You must have these libraries installed in your system globally:

* SDL 1.2
* Boost (any recent version should be ok)

To compile projet you also need:

For Windows
-----------

* Visual Studio 2010

For Linux/Mac OS X
------------------

* CMake
* GNU make


Compilation and run
===================

Windows + Visual Studio 2010
----------------------------

1. Open `VS2010/rmit2012_i3d.sln` solution file
2. In project Configuration Properties/Debugging set Working Directory (for All Configurations) to `$(ProjectDir)../data`
3. Hit `Debug->Start Debugging` menu entry.

Linux/Mac OS X
--------------

1. In project directory create subdirectory called `build`.
1. Go to subdirectory `build` and execute `cmake ../unix/ && make`.
1. If previous step succeeded, go to subdirectory `data` in project directory and execute `../build/main`

Key bindings
============
* `ESC` and `q` - quit
* `w` - wireframe mode
* `a` - toggle axes (world origin, water origin, ship origin, ship origin with roll/pitch applied)
* `n` - toggle normals
* `UP` and `DOWN` - increase and decrease speed (ship does not slow by itself!)
* `LEFT` and `RIGHT` - turn the ship, works only when ship is moving forward
* `-` and `+` - halve/double water tesselation level