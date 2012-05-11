Requirements
============

You must have these libraries installed in your system globally:

* SDL 1.2
* Boost (any recent version should be ok)
* Qt 4.x >= 4.5

To compile projet you also need:

For Windows
-----------

* Visual Studio 2010

For Linux/Mac OS X
------------------

* qmake
* GNU make


Compilation and run
===================

`PATH_TO_PROJECT_DIRECTORY` - if you see that value somewhere, you should replace it with path to project source directory (preferably absolute)

Windows + Visual Studio 2010
----------------------------

1. Create `build` directory *outside* project directory.
1. Using Qt Console enter `build` directory and execute `qmake -spec win32-msvc2010 -tp vc PATH_TO_PROJECT\rmit2012_i3d.pro`
1. Open `rmit2012_i3d.vcxproj` file.
1. In project Configuration properties/Debugging panel set Command Arguments (for all configurations) to `--gamedir PATH_TO_PROJECT_SOURCE_DIRECTORY`
1. Hit `Debug->Start Debugging` menu entry.

Linux/Mac OS X
--------------

1. Create `build` directory *outside* project directory.
1. Enter `build` directory and execute `qmake PATH_TO_PROJECT\rmit2012_i3d.pro && make`
1. Start application by `./rmit2012_i3d --gamedir PATH_TO_PROJECT_DIRECTORY`.

Key bindings
============
* `ESC` and `q` - quit
* `w` - wireframe mode
* `a` - toggle axes (world origin, water origin, ship origin, ship origin with roll/pitch applied)
* `n` - toggle normals
* `UP` and `DOWN` - increase and decrease speed (ship does not slow by itself!)
* `LEFT` and `RIGHT` - turn the ship, works only when ship is moving forward
* `-` and `+` - halve/double water tesselation level
