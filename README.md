Magnificent Game Engine (2.0)
=====================================

History
------------------------------------

This is my second attempt at writing a 2D game engine. Last time I did, I ended up
using DirectX for rendering, FMOD for sound, Box2D for physics, Shiny as
profiler, and tinyXML for loading settings from files. This time around, I have
decided to keep some elements from the previous version of MGE, and change other
elements, which has prompted for a rewrite almost from scratch.

Changes
------------------------------------

This time around I wanted a game engine that could perform on both linux and
windows. Therefore, every descision would be made with cross-platform
suitability in mind. 

There are the new specifications:

* <b>OpenGL</b>: The biggest change this time is my use of OpenGL for rendering, 
  which is the only obvious choice, apart from all the available abstractions of
  OpenGL. I chose to go with native OpenGL becase I wanted the experience of
  having done it. It might be easier with something like Allegro or SDL, but my
  interest for learning OpenGL ultimately made me settle for native OpenGL.

* <b>Qt</b>: I wanted to implement a proper game loop, something that proved
  difficult when using GLUT. I have previously been in touch with small game
  projects using Qt and its OpenGL widget with great success. Qt does not have
  native support for something like a game loop, but it can be done with
  non-standard approaches. Additionally, Qt also icludes qmake, which is a
  useful utility to create project files for MSVC and makefiles when using Qt. 

* <b>FMOD</b>: FMOD is made for games, it supports a vast amount of sound cards,
 and has a very nice api, so it's difficult to find a reason not to use it.
 The generous licensing is also a huge plus. See <a href="www.fmod.org">this
 page</a> for more information on FMOD.

* <b>Squirrel</b> (TODO): Squirrel is a scripting language made for real-time
  applications like games. By utilizing this, it makes game development on MGE2
  much quicker. See <a href="www.squirrel-lang.org">this page</a> for more
  information on Squirrel.

* <b>Shiny</b> (TODO): Shiny did a good job on profiling my last engine, so I
  decided to use it once more to ensure MGE2 performs well.

* <b>DevIl</b> (In progress): A very nice imaging library which works very well
  with OpenGL. Will be used to load textures.

Goals
----------------------------------

* To work on both linux ans windows, promoting the creation of cross-platform
  games

* Have a modular approach; make it easy to change eg. the sound system without
  changing too much of the actual game code.

* Provide a boiler plate for rapid development of new games on both linux and
  windows.

Compilation
---------------------------------

<b>Requirements:</b>
* Qt sdk, if using msvc2010 it must be compiled from source
* FMOD sdk, qmake project file depends on FMOD\_DIR being set to the FMOD root
  directory (containing the folders inc and lib).
* DevIL, qmake project file depends on DEVIL\_DIR being set to the DevIl root
  directory (containing include and lib)

<b>Windows:</b>
Run _qmake -tp vc_ in the directory containing mge2.pro. If desired, set
QMAKESPEC=win32-msvc2010 or otherwise if necessary. Open the project file, save
the solution, and build.

<b>Linux (only tested on Ubuntu):</b>
Run _qmake && make_ in the directory containing mge2.pro. Some dependencies,
such as FMOD, has the version number in the file name, this might require
changes to the .pro file depending on what version of FMOD you're using.

Final notes & contact
--------------------------------
This project is in its early stages and things are bound to change, possibly a
lot. If you want to help out in any way, feel free to fork it, but you should
contact me if you plan on doing something major, of if you just have comments 
or questions. I can be reached at dale.oystein -at- gmail -dot- com.

- Øystein Dale, 2011
