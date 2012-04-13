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

Open `VS2010/rmit2012_i3d.sln` solution file and hit `Debug->Start Debugging` menu entry.

Linux/Mac OS X
--------------

1. In project directory create subdirectory called `build`.
1. Go to subdirectory `build` and execute `cmake ../unix/ && make`.
1. If previous step succeeded, go to subdirectory `data` in project directory and execute `../build/main`
