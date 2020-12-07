# VRR_Flicker_Test_OpenGL
Test application to demonstrate VRR flicker and gamma shift. The program creates a fullscreen OpenGL context on the primary monitor in the desktop resolution, renders a gradient from mid grey to black and then varies the frametime up and down between 1/120th and 1/40th of a second. 

The unstable frame rate should trigger flickering on an LG CX OLED display. 

It was just put together quickly in an afternoon based on an idea I had to create a reproducable way of demonstrating the issue. Given the simplicty of the graphics I just used basic OpenGL without any need for shaders. 

GLFW is used for handling the OpenGL context and window creation. GLEW was added in case of future requirements. 

# Instructions
 Download the zip, extract and run the exe. It will launch a full screen gradient display that is static. With VRR on the LG CX you should see this flicker. 
 
 Press Escape to exit. 
