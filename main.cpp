#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>
#include <random>

// macros
#define PI 3.14159265358979323846

#define APP_WIDTH 720
#define APP_HEIGHT 960

#define FLORAL_COUNT 200
#define FLORAL_MARGIN 20
#define FLORAL_SIZE_MIN 8
#define FLORAL_SIZE_MAX 12
#define FLORAL_ANIMATION_SPEED .008

typedef struct {
    GLfloat x, y;
} Point;

typedef struct {
    Point position;
    float size;
    float rotation;
    float color[3];
    float alpha;
    enum Shape {
        TRIANGLE, CIRCLE, SQUARE
    } shape;
    bool fading;
} floral;

// function declarations

int random_int(int, int);

float random_float(float, float);

bool random_bool();

void use_absolute_cs();

void use_percentage_cs();

void display();

void reshape_handler(int, int);

void timer_handler(int);

void keyboard_handler(unsigned char, int, int);

void special_handler(int, int, int);

void mouse_handler(int, int, int, int);

void motion_handler(int, int);

void init_floral();

void draw_floral(floral *);

void floral_animate();

void draw_background();

void draw_pointer();

// global vars
Point mouse_pointer = {0, 0};
floral bk_decorations[FLORAL_COUNT];
enum Color {
    PINK, BLUE, YELLOW, GREEN, WHITE
} bk_color = WHITE;

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
    glutSpecialFunc(special_handler);
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
//    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    draw_background();
    draw_pointer();

    glutSwapBuffers();
}

void keyboard_handler(unsigned char key, int, int) {
    switch (key) {
        case 'q':
        case 'Q':
            exit(0);
        case 'b':
        case 'B':
            if (bk_color != PINK) {
                bk_color = static_cast<Color>(static_cast<int>(bk_color) - 1);
            } else {
                bk_color = WHITE;
            }
            break;
        case 'n':
        case 'N':
            if (bk_color != WHITE) {
                bk_color = static_cast<Color>(static_cast<int>(bk_color) + 1);
            } else {
                bk_color = PINK;
            }
            break;
    }
}

void special_handler(int key, int, int) {
    switch (key) {
        case GLUT_KEY_UP:
        case GLUT_KEY_LEFT:
            if (bk_color != PINK) {
                bk_color = static_cast<Color>(static_cast<int>(bk_color) - 1);
            } else {
                bk_color = WHITE;
            }
            break;
        case GLUT_KEY_DOWN:
        case GLUT_KEY_RIGHT:
            if (bk_color != WHITE) {
                bk_color = static_cast<Color>(static_cast<int>(bk_color) + 1);
            } else {
                bk_color = PINK;
            }
            break;
    }
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
    floral_animate();
    glutPostRedisplay();    // redraw content
    glutTimerFunc(1000 / 60, timer_handler, 0);
}

void draw_background() {
    use_percentage_cs();
    glBegin(GL_POLYGON);
    switch (bk_color) {
        case PINK:
            glColor4f(255 / 255.0, 196 / 255.0, 216 / 255.0, 1);
            glVertex2f(0, 1);
            glVertex2f(1, 1);
            glColor4f(255 / 255.0, 220 / 255.0, 231 / 255.0, 1);
            glVertex2f(1, 0);
            glVertex2f(0, 0);
            break;
        case BLUE:
            glColor4f(132 / 255.0, 209 / 255.0, 250 / 255.0, 1);
            glVertex2f(0, 1);
            glVertex2f(1, 1);
            glColor4f(201 / 255.0, 243 / 255.0, 255 / 255.0, 1);
            glVertex2f(1, 0);
            glVertex2f(0, 0);
            break;
        case YELLOW:
            glColor4f(245 / 255.0, 228 / 255.0, 152 / 255.0, 1);
            glVertex2f(0, 1);
            glVertex2f(1, 1);
            glColor4f(255 / 255.0, 249 / 255.0, 214 / 255.0, 1);
            glVertex2f(1, 0);
            glVertex2f(0, 0);
            break;
        case GREEN:
            glColor4f(189 / 255.0, 217 / 255.0, 152 / 255.0, 1);
            glVertex2f(0, 1);
            glVertex2f(1, 1);
            glColor4f(233 / 255.0, 252 / 255.0, 199 / 255.0, 1);
            glVertex2f(1, 0);
            glVertex2f(0, 0);
            break;
        case WHITE:
            glColor4f(235 / 255.0, 235 / 255.0, 235 / 255.0, 1);
            glVertex2f(0, 1);
            glVertex2f(1, 1);
            glColor4f(255 / 255.0, 255 / 255.0, 255 / 255.0, 1);
            glVertex2f(1, 0);
            glVertex2f(0, 0);
            break;
    }
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
    glTranslatef(f->position.x, f->position.y, 0);  // move to new cs
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
        ptr->position.x = random_int(FLORAL_MARGIN, APP_WIDTH - FLORAL_MARGIN);
        ptr->position.y = random_int(FLORAL_MARGIN, APP_HEIGHT - FLORAL_MARGIN);
        // random size
        ptr->size = random_float(FLORAL_SIZE_MIN, FLORAL_SIZE_MAX);
        // random rotation
        ptr->rotation = random_float(-180, 180);
        // random color
//        ptr->color[0] = random_float(0, 1);
//        ptr->color[1] = random_float(0, 1);
//        ptr->color[2] = random_float(0, 1);
        switch (random_int(0, 4)) {
            case 0:
                ptr->color[0] = 224 / 255.0;
                ptr->color[1] = 27 / 255.0;
                ptr->color[2] = 57 / 255.0;
                break;
            case 1:
                ptr->color[0] = 235 / 255.0;
                ptr->color[1] = 100 / 255.0;
                ptr->color[2] = 28 / 255.0;
                break;
            case 2:
                ptr->color[0] = 227 / 255.0;
                ptr->color[1] = 180 / 255.0;
                ptr->color[2] = 9 / 255.0;
                break;
            case 3:
                ptr->color[0] = 18 / 255.0;
                ptr->color[1] = 130 / 255.0;
                ptr->color[2] = 46 / 255.0;
                break;
            case 4:
                ptr->color[0] = 9 / 255.0;
                ptr->color[1] = 52 / 255.0;
                ptr->color[2] = 145 / 255.0;
                break;
        }
        // random alpha
        ptr->alpha = random_float(.2, 1);
        // random Shape
        ptr->shape = static_cast<floral::Shape>(rand() % (floral::SQUARE + 1));
        ptr->fading = random_bool();
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

bool random_bool() {
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> distribution(0, 1);
    return distribution(generator);
}

void floral_animate() {
    floral *ptr = bk_decorations;
    floral *end_ptr = bk_decorations + sizeof(bk_decorations) / sizeof(bk_decorations[0]);
    while (ptr < end_ptr) {
        if (ptr->fading) {
            ptr->alpha -= FLORAL_ANIMATION_SPEED;
            if (ptr->alpha < 0) {
                ptr->alpha = 0;
                ptr->fading = false;
                // random position on faded
                ptr->position.x = random_int(FLORAL_MARGIN, APP_WIDTH - FLORAL_MARGIN);
                ptr->position.y = random_int(FLORAL_MARGIN, APP_HEIGHT - FLORAL_MARGIN);
            }
        } else {
            ptr->alpha += FLORAL_ANIMATION_SPEED;
            if (ptr->alpha > 1) {
                ptr->alpha = 1;
                ptr->fading = true;
            }
        }
        ptr++;
    }
}
