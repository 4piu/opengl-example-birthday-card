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

typedef struct {
    point coordinator;
    float size;
    float rotation;
    float color[3];
    float alpha;
    enum shape {
        TRIANGLE, CIRCLE, SQUARE
    } shape;
} floral;

void use_absolute_cs();

void use_percentage_cs();

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

    // enable blend (rgba support)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
            (float) x / (float) glutGet(GLUT_WINDOW_WIDTH) * APP_WIDTH;
    mouse_pointer.y =
            (float) y / (float) glutGet(GLUT_WINDOW_HEIGHT) * APP_HEIGHT;
}

void reshape_handler(int w, int h) {
    glViewport(0, 0, w, h); // set viewport
}

void timer_handler(int) {
    glutPostRedisplay();    // redraw content
    glutTimerFunc(1000 / 60, timer_handler, 0);
}

void draw_background() {
    use_percentage_cs();
    glBegin(GL_POLYGON);
    glColor4f(255 / 255.0, 196 / 255.0, 216 / 255.0, 1);
    glVertex2f(0, 1);
    glVertex2f(1, 1);
    glColor4f(255 / 255.0, 220 / 255.0, 231 / 255.0, 1);
    glVertex2f(1, 0);
    glVertex2f(0, 0);
    glEnd();

    //TODO test
    floral f0 = {
            point{360, 100},
            5,
            45,
            {1, 0, 0},
            .5,
            floral::SQUARE
    };
    floral f1 = {
            point{360, 300},
            5,
            45,
            {0, 1, 0},
            .5,
            floral::TRIANGLE
    };
    floral f2 = {
            point{360, 600},
            5,
            0,
            {0, 0, 1},
            .5,
            floral::CIRCLE
    };
    draw_floral(f0);
    draw_floral(f1);
    draw_floral(f2);
}

void draw_pointer() {
    // TODO
    use_absolute_cs();
    glColor4f(255 / 255.0, 255 / 255.0, 255 / 255.0, 1);
    glPointSize(10);
    glBegin(GL_POINTS);
    glVertex2f(mouse_pointer.x, mouse_pointer.y);
    glEnd();
}

void draw_floral(floral f) {
    glPushMatrix(); // save previous cs
    glTranslatef(f.coordinator.x, f.coordinator.y, 0);  // move to new cs
    glRotatef(f.rotation, 0, 0, 1); // rotate cs

    glColor4f(f.color[0], f.color[1], f.color[2], f.alpha);
    use_absolute_cs();
    switch (f.shape) {
        case floral::SQUARE:
            glBegin(GL_POLYGON);
            glVertex2f(-f.size, f.size);
            glVertex2f(f.size, f.size);
            glVertex2f(f.size, -f.size);
            glVertex2f(-f.size, -f.size);
            break;
        case floral::TRIANGLE:
            glBegin(GL_TRIANGLES);
            glVertex2f(-f.size * std::sqrt(3) / 2, -f.size / 2);
            glVertex2f(f.size * std::sqrt(3) / 2, -f.size / 2);
            glVertex2f(0, f.size);
            break;
        case floral::CIRCLE:
            glBegin(GL_POINTS);
            glEnable(GL_POINT_SMOOTH);
            glPointSize(f.size);
            glVertex2f(0, 0);
            break;
    }
    glEnd();

    glPopMatrix();  // restore previous cs
}

void use_absolute_cs() {
    glMatrixMode(GL_PROJECTION); // change matrix mode
    glLoadIdentity();   // reset projection matrix
    gluOrtho2D(0, APP_WIDTH, APP_HEIGHT, 0); // use custom coordination system
    glMatrixMode(GL_MODELVIEW);
}

void use_percentage_cs() {
    glMatrixMode(GL_PROJECTION); // change matrix mode
    glLoadIdentity();   // reset projection matrix
    gluOrtho2D(0, 1, 1, 0); // use custom coordination system
    glMatrixMode(GL_MODELVIEW);
}

