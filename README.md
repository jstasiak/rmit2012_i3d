Requirements
============

You must have these libraries installed in your system globally:

* SDL 1.2
* SDL_ttf
* SDL_image
* Boost (any recent version should be ok)

To compile projet you also need:

For Linux/Mac OS X
------------------

* cmake
* GNU make


Compilation and run
===================


Linux/Mac OS X
--------------

1. In project directory create subdirectory called `build`.
1. Go to subdirectory `build` and execute `cmake ../ && make`
1. If previous step succeeded, enter `./main --gamedir ../data`

Additional Features
===================

* skybox
* infinite water and terrain
* perlin-noise terrain

Key bindings
============
* `ESC` and `Q` - quit
* `T` - wireframe mode
* `N` - toggle normals
* `-` and `+` - halve/double water tesselation level
* ARROW KEYS + PAGE DOWN + DELETE - player 1 controls
* W/S/A/D + Q + E - player 2 controls
* `ENTER` - game reset
