#include <GL/glew.h>
#include <GL/glut.h>

void helloWorld();

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(320, 320);
    glutCreateWindow("Hello World");
    glutDisplayFunc(helloWorld);
    glutMainLoop();
    return 0;
}

void helloWorld() {
    // Clear the buffer to the predefined color and depth value
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //Move origin of current user coordinate system to the screen center: similar to a reset operation
    glLoadIdentity();
    glBegin(GL_TRIANGLES); // Start draw TRIANGLE function
    glVertex3f(-0.5, -0.5, 0.0); // Set coordinates of first vertex
    glVertex3f(0.5, -0.5, 0.0); // Set coordinates of second vertex
    glVertex3f(0.0, 0.5, 0.0); // Set coordinates of last vertex
    glEnd(); // End draw TRIANGLE function
    glutSwapBuffers(); // Refresh the screen to display the graphics
}