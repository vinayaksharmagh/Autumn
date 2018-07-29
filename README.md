# Autumn
This project was created using OpenGL API . It creates a region of randomly scattered particles (here leafs) which are made to fall from top of display to bottom in an animated manner.Once a particle cross the bottom threshold of screen, it is pooped from there and repositioned at some offset on top so that it can again drop down as a new particle.

Other External Libruaries used

-GLM

-GLFW

-GLAD

-stb_image

-ktx loader


file info

-main.cpp   : simply contains entry point and call to Opengl applcation part

-app.cpp    : contains all the definitions of methods needed for Opengl application part

-context.cpp: deals with creation of window using glfw methods

-abst.h     : declares an abstract class( named "abst")  with a pure virtual function( named "pure" ) 

-conc.cpp   : defines all the concrete classes (inherited from abstract class "abst" ) .This file is critical as it contains various 
              overridden definitions of function "pure" which is binded dynamically from different parts of code in application
              (This has been done to partition code logically without having to create many different functions)
              {Yes, I know that dynamic invocation of function is slower than static one but I have done such partition for 
              ease of coding and debugging (just like error handling is done despite its known effects on performance of execution).
              In the final release code (for building exe), dynamic binding calls can be replaced by corrosponding definitions in                       conc.cpp}

-resfun.cpp : contains definition of various funtions other than member functions of class app. {only a few of these functions have been used in this project (these functions are infact custom abstractions used by coder of this project for his other opengl projects)

-glad.c     : required by GLAD libruary

-stb_image.cpp : required by stb_image libruary

-Vertex.txt  : Vertex shader code, it contains trasformation matricies, uniform blocks and carries out trasformation of vertex position                 and then sets it to gL_Position. It also sends texture coordinates to fragment shader.
-Fragment.txt: Fragment shader code, obtains texture coordinates from vertex shader. Apart from this it also recieves 2D texture array.
             
