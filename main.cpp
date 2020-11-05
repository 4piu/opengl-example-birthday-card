#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>

// macros

#define APP_WIDTH 720
#define APP_HEIGHT 960

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

// function declarations

void use_absolute_cs();

void use_percentage_cs();

void display();

void reshape_handler(int, int);

void timer_handler(int);

void keyboard_handler(unsigned char, int, int);

void mouse_handler(int, int, int, int);

void motion_handler(int, int);

void draw_floral(floral);

void draw_background();

void draw_pointer();

// global vars
point mouse_pointer = {0, 0};

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(APP_WIDTH, APP_HEIGHT);  // set window size
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - APP_WIDTH) / 2,
                           (glutGet(GLUT_SCREEN_HEIGHT) - APP_HEIGHT) / 2); // center the window
    glutCreateWindow("Happy Birthday!");    // window title
    glutSetCursor(GLUT_CURSOR_NONE);    // hide cursor

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

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    draw_background();
    draw_pointer();

    glutSwapBuffers();
}

void keyboard_handler(unsigned char key, int x, int y) {
    // TODO
}

void mouse_handler(int btn, int state, int x, int y) {
    // TODO
}

void motion_handler(int x, int y) {
    // track mouse pointer position
    mouse_pointer.x =
            (float) x / (float) glutGet(GLUT_WINDOW_WIDTH) * APP_WIDTH;
    mouse_pointer.y =
            (float) y / (float) glutGet(GLUT_WINDOW_HEIGHT) * APP_HEIGHT;
}

void reshape_handler(int w, int h) {
    int new_w = w, new_h = h;
    if ((float) w / (float) h > (float) APP_WIDTH / (float) APP_HEIGHT) {
        new_w = (int) ((float) h * (float) APP_WIDTH / (float) APP_HEIGHT);
    } else if ((float) w / (float) h < (float) APP_WIDTH / (float) APP_HEIGHT) {
        new_h = (int) ((float) w / (float) APP_WIDTH * (float) APP_HEIGHT);
    }
    glViewport((w - new_w) / 2, (h - new_h) / 2, new_w, new_h); // set viewport
    glutReshapeWindow(new_w, new_h);    // force shape window to specified ratio
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

