==============================================================================
libbkmeans: A Library for clustering with K-means on binary data
==============================================================================
Manuel J. Marin-Jimenez

This software implements a kmeans clustering method for binary data.
This can be used to cluster binary features as ORB, FREAK, etc.
--------------------------------------------------------------------------------

   Source-Code:   https://github.com/mjmarin/libbkmeans

--------------------------------------------------------------------------------
Contents of the package:
--------------------------------------------------------------------------------
- bin - the executable file will be created here
- data - some test images
- doc - folder where the documentation will be generated
- include - contains all the software header files
- libs - folder where the library will be generated
- obj - folder where the object files will be generated during the compilation
- src - contains all the software source files
- tests - contains some programs to test the library

--------------------------------------------------------------------------------
Requirements:
--------------------------------------------------------------------------------
This software has been tested on MS Windows with MS Visual C++ 2010 Express and OpenCV 2.4.4

--------------------------------------------------------------------------------
Quick start:
--------------------------------------------------------------------------------
1. Compile with CMake
2. Run test program:
   clustercv data\plant.jpg 100

--------------------------------------------------------------------------------
Citation:
--------------------------------------------------------------------------------
If you use this library for your publications, please cite it as:
@misc{libbkmeans,
 author = {Marin-Jimenez, Manuel J.},
 title = {{LibBKmeans}: A Library for clustering with K-means on binary data},
 year = {2013},
 note =   {Software available at \url{https://github.com/mjmarin/libbkmeans}}
}

--------------------------------------------------------------------------------
Version history:
--------------------------------------------------------------------------------
 - v0.0.1: first release
 