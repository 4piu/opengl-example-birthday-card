#include <GL/glew.h>
#include <GL/glut.h>

#define APP_WIDTH 720
#define APP_HEIGHT 960

#define ORTHO_L -100.0
#define ORTHO_R 100.0
#define ORTHO_B -100.0
#define ORTHO_T 100.0

typedef struct {
    GLfloat x, y;
} point;

void display();

void reshape_handler(int, int);

void timer_handler(int);

void keyboard_handler(unsigned char, int, int);

void mouse_handler(int, int, int, int);

void motion_handler(int, int);

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
    glutMotionFunc(motion_handler);
    glutPassiveMotionFunc(motion_handler);

    glutMainLoop();
    return 0;
}

point mouse_pointer = {0, 0};

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glPointSize(10);
    glBegin(GL_POINTS);
    glVertex2f(mouse_pointer.x, mouse_pointer.y);
    glEnd();

    glutSwapBuffers();
}

void keyboard_handler(unsigned char key, int x, int y) {

}

void mouse_handler(int btn, int state, int x, int y) {

}

void motion_handler(int x, int y) {
    // track mouse pointer position
    mouse_pointer.x =
            (float) x / (float) glutGet(GLUT_WINDOW_WIDTH) * (ORTHO_R - ORTHO_L) - (ORTHO_R - ORTHO_L) / 2;
    mouse_pointer.y =
            (1 - (float) y / (float) glutGet(GLUT_WINDOW_HEIGHT)) * (ORTHO_T - ORTHO_B) - (ORTHO_T - ORTHO_B) / 2;
}

void reshape_handler(int w, int h) {
    glViewport(0, 0, w, h); // set viewport
    glMatrixMode(GL_PROJECTION); // change matrix mode
    glLoadIdentity();   // reset projection matrix
    gluOrtho2D(ORTHO_L, ORTHO_R, ORTHO_B, ORTHO_T); // use custom coordination system
    glMatrixMode(GL_MODELVIEW);
}

void timer_handler(int) {
    glutPostRedisplay();    // redraw content
    glutTimerFunc(1000 / 60, timer_handler, 0);
}

