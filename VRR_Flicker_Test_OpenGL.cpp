// VRR_Flicker_Test_OpenGL.cpp: The main source file for this project
// This project is designed to demonstrate VRR related gamma shift and flicker on displays, specifically the LG CX
// The idea is to create a simple OpenGL context and fullscreen output with a gradient and then refresh the display at varying intevals 
#include <GL/glew.h> // include headers for GLEW for accessing modern OpenGL features, if required
#include <GLFW/glfw3.h> // include headers for GLFW for creating the OpenGL context and the window management etc
#include <stdlib.h> // standard library headers for basic C functions
#include <stdio.h> // standard IO for stuff like printing text (printf)
#include <Windows.h> // Windows header for sleeping the loop

// Define the GLFW error callback function - function that is called to print an error when one occurs
void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

// Define the callback function for handling key input
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Flag the window for closure when the escape key is pressed
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

// Generate a random number between the lower and upper values
float random_between_limits (float lower, float upper) {
    float random = ((float)rand()) / (float)RAND_MAX;
    float diff = upper - lower;
    float scaled = random * diff;
    return lower + scaled;
}

// The main function that runs when the program starts
int main()
{
    // Try to initialise GLFW 
    if (!glfwInit())
    {
        // Initialization failed
        printf ("GLFW initialisation failed!\n");
        exit(1);
    }

    // Set the error callback function to the one that was defined
    glfwSetErrorCallback(error_callback);

    // Create the GLFW OpenGL context and create a fullscreen view on the primary monitor at the current resolution
    GLFWwindow* window = glfwCreateWindow(glfwGetVideoMode(glfwGetPrimaryMonitor())->width, glfwGetVideoMode(glfwGetPrimaryMonitor())->height, "VRR gamma shift and flicker test", glfwGetPrimaryMonitor(), NULL);
    if (!window)
    {
        // Window creation failed
        printf("GLFW window creation failed. Resolution may no be supported?\n");
        glfwTerminate();
        exit(1);
    }

    // Make the OpenGL context from the window that was created active
    glfwMakeContextCurrent(window);

    // Disable v-sync
    glfwSwapInterval(0);

    // Set the key callback function for the main window
    glfwSetKeyCallback(window, key_callback);

    // Start timer for counting frame time
    glfwSetTime(0.0);
    // Use double for the precision as float might lose precision too quickly
    // I haven't actually checked the precision but for the amount of time this program will run for I'm sure it'll be fine
    double previous = 0.0;

    // Set the initial value for the frame time we want to achieve - this will vary over time to achieve the variable frame time
    // Start at 1/120th of a second for 120 Hz
    float frame_time = 1.0 / 120.0;

    // store the starting frame time so we can calculate the next frame time
    float prev_frame_time = frame_time;

    // set how much each frame time should change by between frames
    float frame_time_delta = 1.0 / 1000.0;

    // Keep repeating until the Windows is flagged as being closed by whatever event (pressing the X or keyboard input)
    // This is where the actual work is done
    while (!glfwWindowShouldClose(window))
    {

        // This is where the important bit happens
        // V-sync is not enabled for this window so it should loop as fast as the CPU and GPU can handle
        // To avoid this the loop is made to wait for a variable time period which will make the frame time change each time

        // Get the time now and work out how much time has elapsed since the previous iteration
        double now = glfwGetTime();
        float time_delta = now - previous; // the time delta itself should be safe as a float
        previous = now;

        // count down the frame time 
        frame_time = frame_time - time_delta;

        // If the count down has finished then execute the frame buffer swap and reset the timer to a different value
        if (frame_time <= 0.0) {

            // Get the width and height of the OpenGL viewport
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            glViewport(0, 0, width, height);
            // clear the screen each time 
            glClear(GL_COLOR_BUFFER_BIT);

            // Using ancient basic OpenGL without shaders for now
            // set up the matrices for the camera view - just identity as it is a simple fullscreen image
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            // Draw a gradient that fills the screen
            // Set the colour and the 2 vertices for the top left and bottom left then set the colour for the bottom right and top right
            glBegin(GL_QUADS);
            // Start at mid grey (1.0 is max i.e white)
            glColor3f(0.5, 0.5, 0.5);
            glVertex2f(-1.0, 1.0);
            glVertex2f(-1.0, -1.0);
            // end at black
            glColor3f(0.0, 0.0, 0.0);
            glVertex2f(1.0, -1.0);
            glVertex2f(1.0, 1.0);
            glEnd();

            // Push the image we have just drawn to the display
            // the OpenGL context is double buffered so this swaps between the buffer that is displayed and the one that is used for drawing
            glfwSwapBuffers(window);

            // set the next frame time by adding the frame time delta to the previous frame time
            frame_time = prev_frame_time + frame_time_delta;

            // Make frame time random between 2 levels for testing
            //frame_time = random_between_limits(1.0 / 120.0, 1.0 / 40.0);

            // update the previous frame time 
            prev_frame_time = frame_time;


            // If the frame time goes above 1/40th of a second or under 1/120th of a second then invert the frame time delta so that the frame time moves in the opposite direction
            if (frame_time >= 1.0 / 40.0 || frame_time <= 1.0 / 120.0) {
                frame_time_delta = -frame_time_delta;
            }

        }
        // Sleep a little to avoid completely ragging the CPU
        Sleep(0);
        glfwPollEvents();
}

    // Destroy the GLFW window that was created earlier
    glfwDestroyWindow(window);

    // Stop using GLFW
    glfwTerminate();
    // close successfully 
    exit(0);
}
