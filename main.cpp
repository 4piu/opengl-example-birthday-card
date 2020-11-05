#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>
#include <random>
#include <functional>

// macros

#define APP_WIDTH 720
#define APP_HEIGHT 960

#define FLORAL_COUNT 100
#define FLORAL_MARGIN 10
#define FLORAL_SIZE_MIN 3
#define FLORAL_SIZE_MAX 8

typedef struct {
    GLfloat x, y;
} Point;

typedef struct {
    Point coordinator;
    float size;
    float rotation;
    float color[3];
    float alpha;
    enum Shape {
        TRIANGLE, CIRCLE, SQUARE
    } shape;
} floral;

// function declarations

int random_int(int, int);

float random_float(float, float);

void use_absolute_cs();

void use_percentage_cs();

void display();

void reshape_handler(int, int);

void timer_handler(int);

void keyboard_handler(unsigned char, int, int);

void mouse_handler(int, int, int, int);

void motion_handler(int, int);

void init_floral();

void draw_floral(floral *);

void draw_background();

void draw_pointer();

// global vars
Point mouse_pointer = {0, 0};
floral bk_decorations[FLORAL_COUNT];

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

    // msaa
    glEnable(GL_MULTISAMPLE);

    // enable blend (rgba and smooth support)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // init global vars
    init_floral();

    glutMainLoop(); // m4k3 17 r41n
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
    glutReshapeWindow(new_w, new_h);    // force Shape window to specified ratio
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

    floral *ptr = bk_decorations;
    floral *end_ptr = bk_decorations + sizeof(bk_decorations) / sizeof(bk_decorations[0]);
    while (ptr < end_ptr) {
        draw_floral(ptr);
        ptr++;
    }

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

void draw_floral(floral *f) {
    glPushMatrix(); // save previous cs
    glTranslatef(f->coordinator.x, f->coordinator.y, 0);  // move to new cs
    glRotatef(f->rotation, 0, 0, 1); // rotate cs

    glColor4f(f->color[0], f->color[1], f->color[2], f->alpha);
    use_absolute_cs();
    switch (f->shape) {
        case floral::SQUARE:
            glRectf(-f->size / 2, -f->size / 2, f->size / 2, f->size / 2);
            break;
        case floral::TRIANGLE:
            glBegin(GL_TRIANGLES);
            glVertex2f(-f->size * std::sqrt(3) / 2, -f->size / 2);
            glVertex2f(f->size * std::sqrt(3) / 2, -f->size / 2);
            glVertex2f(0, f->size);
            break;
        case floral::CIRCLE:
//            glEnable(GL_POINT_SMOOTH);
//            glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
//            glBegin(GL_POINTS);
//            glPointSize(f->size);
//            glVertex2f(0, 0);
            glBegin(GL_POLYGON);
            for (float i = 0; i < 2 * PI; i += PI / 360) {
                glVertex2f(f->size / 2 * cosf(i), f->size / 2 * sinf(i));
            }
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

void init_floral() {
    floral *ptr = bk_decorations;
    floral *end_ptr = bk_decorations + sizeof(bk_decorations) / sizeof(bk_decorations[0]);
    while (ptr < end_ptr) {
        // random position
        ptr->coordinator.x = random_int(FLORAL_MARGIN, APP_WIDTH - FLORAL_MARGIN);
        ptr->coordinator.y = random_int(FLORAL_MARGIN, APP_HEIGHT - FLORAL_MARGIN);
        // random size
        ptr->size = random_float(FLORAL_SIZE_MIN, FLORAL_SIZE_MAX);
        // random rotation
        ptr->rotation = random_float(-180, 180);
        // random color
        ptr->color[0] = random_float(0, 1);
        ptr->color[1] = random_float(0, 1);
        ptr->color[2] = random_float(0, 1);
        // random alpha
        ptr->alpha = random_float(.2, 1);
        // random Shape
        ptr->shape = static_cast<floral::Shape>(rand() % (floral::SQUARE + 1));
        ptr++;
    }
}

int random_int(int min, int max) {
//    return rand() % (max - min + 1) + min;
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

float random_float(float min, float max) {
//    return min + (float) rand() / (float) (RAND_MAX) * max;
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(generator);
}

