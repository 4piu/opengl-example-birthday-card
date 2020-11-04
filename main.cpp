#include <GL/glew.h>
#include <GL/glut.h>

#define APP_WIDTH 720
#define APP_HEIGHT 960

void display();

void reshape_handler(int, int);

void timer_handler(int);

void keyboard_handler(unsigned char, int, int);

void mouse_handler(int, int, int, int);

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(APP_WIDTH, APP_HEIGHT);  // set window size
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - APP_WIDTH) / 2,
                           (glutGet(GLUT_SCREEN_HEIGHT) - APP_HEIGHT) / 2); // center the window
    glutCreateWindow("Happy Birthday!");    // window title

    // set callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape_handler);
    glutTimerFunc(0, timer_handler, 0);
    glutKeyboardFunc(keyboard_handler);
    glutMouseFunc(mouse_handler);

    glutMainLoop();
    return 0;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glBegin(GL_TRIANGLES);
    glVertex2d(-10, -10);
    glVertex2d(10, -10);
    glVertex2d(0, 8);
    glEnd();

    glutSwapBuffers();
}

void keyboard_handler(unsigned char key, int x, int y) {

}

void mouse_handler(int btn, int state, int x, int y) {

}

void reshape_handler(int w, int h) {
    glViewport(0, 0, w, h); // set viewport
    glMatrixMode(GL_PROJECTION); // change matrix mode
    glLoadIdentity();   // reset projection matrix
    gluOrtho2D(-100, 100, -100, 100);
    glMatrixMode(GL_MODELVIEW);
}

void timer_handler(int) {
    glutTimerFunc(1000 / 60, timer_handler, 0);
}

