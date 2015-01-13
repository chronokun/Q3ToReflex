Q3ToReflex
==========

Map converter to convert q3 format .map files to reflex format .map files

Usage run from commandline: Q3ToReflex.exe <input>.map <output>.map <switches>

Available switches are:
-nopatches
-notrigger
-noclip
-tessfactor <1-7>
-allcaulk

Notes:

	- Currently does not convert entities.

	- Most material names are copied unchanged from input map.

	- Texture offsets, scale and rotations may not be correct.

	- The parser is still quite flaky.

	- No GUI.

	- Code could do with some tidying, refactoring and commenting.