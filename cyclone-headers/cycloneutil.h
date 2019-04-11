#ifndef CYCLONE_UTIL_H
#define CYCLONE_UTIL_H

/**
 * New in the 2019 refactor: this header contains utility methods
 * which perform setup tasks which used to be hardcoded in demos/main.cpp
 */

#include "ogl_headers.h"
#include "cyclone/app.h"
#include "cyclone/timing.h"

Application* __cyclone_app_ptr;

/**
 * Creates a window in which to display the scene.
 */
void createWindow(const char* title)
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(640,320);
    glutInitWindowPosition(0,0);
    glutCreateWindow(title);
}

/**
 * Called each frame to update the 3D scene. Delegates to
 * the application.
 */
void update()
{
    // Update the timing.
    TimingData::get().update();

    // Delegate to the application.
    __cyclone_app_ptr->update();
}

/**
 * Called each frame to display the 3D scene. Delegates to
 * the application.
 */
void display()
{
    __cyclone_app_ptr->display();

    // Update the displayed content.
    glFlush();
    glutSwapBuffers();
}

/**
 * Called when a mouse button is pressed. Delegates to the
 * application.
 */
void mouse(int button, int state, int x, int y)
{
    __cyclone_app_ptr->mouse(button, state, x, y);
}

/**
 * Called when the display window changes size.
 */
void reshape(int width, int height)
{
    __cyclone_app_ptr->resize(width, height);
}

/**
 * Called when a key is pressed.
 */
void keyboard(unsigned char key, int x, int y)
{
    // Note we omit passing on the x and y: they are rarely needed.
    __cyclone_app_ptr->key(key);
}

/**
 * Called when the mouse is dragged.
 */
void motion(int x, int y)
{
    __cyclone_app_ptr->mouseDrag(x, y);
}

void applicationLauncher(Application* app) {
    __cyclone_app_ptr = app;
    
    // Set up GLUT and the timers
    char* fake_argv = {""};
    int argc = 1;
    glutInit(&argc, &fake_argv);
    TimingData::init();

    // Create the application and its window
    createWindow(__cyclone_app_ptr->getTitle());

    // Set up the appropriate handler functions
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutIdleFunc(update);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    // Run the application
    __cyclone_app_ptr->initGraphics();
    glutMainLoop();

    // Clean up the application
    __cyclone_app_ptr->deinit();
    delete __cyclone_app_ptr;
    TimingData::deinit();
}

#endif