Eddy Chan (chane)
December 11, 2008
ECS 175
Assignment #4 (Final)

Table of Content
================
1. New Commands
	1.1 Clipping
2. 'Cessna' Flight Control
3. Compilation
4. Running

1. New Commands
---------------
1.1 Surface - surface <obj_name> <texture_file> <num_subdivisions> <16-3d control points>
		Generate a textured surface using the Bezier patch method. Subdivions are performed
		<num_subdivisions> times. The 16 control points form a 4x4 matrix.

2. 'Cessna' Flight Control
--------------------------
Increase speed - Scroll wheel up
Decrease speed - Scroll wheel down
Turn left/right - Hold down right button and move mouse left/right (note: this will 
					also have an accompanying rolling motion)
Pitch - Hold down left mouse button and move mouse up/down

3. Compilation
--------------
Note: This program uses the the Boost C++ library. Boost >1.3.4 is required.
'qmake' (or 'qmake-qt4')
and 
'make'
should generally work.

4. Running
----------
'midiasview' is the executable compiled. Midas commands can be entered
interactively when program starts or they can be redirected from a file.
The project directory contains several .poly files, as listed here:

final.poly (slow, generates all objects)
final2.poly (fast, objects already generated)

The easiest way to view them all is to run the 'runall.sh' script.
To see individual graphics, use './midiasview < {filename}'. Passing filename as 
argument is not supported at the moment.
Graphical objects are generated into the ./poly directory.

