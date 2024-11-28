#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <stack>
#include <time.h>
#define xpix 500
#include <cstring>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

float r, g, b, x, y;
bool flag = true;
int x2, y2, x3, y3, x4, y4, x5, y5;
int a1, b1;
int a2, b2;
int a3, b3;
int a4, b4;


int counter = 1;
char buffer[20];

void renderBitmapString(float x, float y, void* font, const string& str) {
    glRasterPos2f(x, y);
    glColor3f(1.0f, 1.0f, 1.0f);
    for (char c : str) {
        glutBitmapCharacter(font, c);
    }
}

void delay(float ms) {
    clock_t goal = ms + clock();
    while (goal > clock());
}
void init() {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glColor3f(0.0, 0.0, 0.0);
    glPointSize(1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
}

void bound_it(int x, int y, float* fillColor, float* bc) {
    float color[3];
    glReadPixels(x, y, 1.0, 1.0, GL_RGB, GL_FLOAT, color);
    if ((color[0] != bc[0] || color[1] != bc[1] || color[2] != bc[2]) && (
        color[0] != fillColor[0] || color[1] != fillColor[1] || color[2] != fillColor[2])) {
        glColor3f(fillColor[0], fillColor[1], fillColor[2]);
        glBegin(GL_POINTS);
        glVertex2i(x, y);
        glEnd();
        glFlush();
        bound_it(x + 1, y, fillColor, bc);
        bound_it(x - 2, y, fillColor, bc);
        bound_it(x, y + 2, fillColor, bc);
        bound_it(x, y - 2, fillColor, bc);
    }
}

void mouseboundaryfill(int btn, int state, int x, int y) {
    y = 600 - y;
    if (btn == GLUT_RIGHT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            float bCol[] = { 1,0,0 };
            float color[] = { 0,0,1 };
            bound_it(x, y, color, bCol);
        }
    }
    if (btn == GLUT_LEFT_BUTTON)
    {

        if (state == GLUT_DOWN)
        {
            if (counter == 1)
            {
                a1 = x;
                b1 = y;
                counter++;
            }
            else if (counter == 2)
            {

                a2 = x;
                b2 = y;
                counter++;
            }
            else if (counter == 3)
            {
                a3 = x;
                b3 = y;
                counter++;
            }
            else if (counter == 4)
            {
                a4 = x;
                b4 = y;
                counter = 0;
            }
        }
    }
}

void world() {
    glLineWidth(3);
    glPointSize(2);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(a1, b1);
    glVertex2f(a2, b2);
    glVertex2f(a3, b3);
    glVertex2f(a4, b4);
    glEnd();
    glColor3f(1, 1, 1);
    renderBitmapString(50, 20, GLUT_BITMAP_HELVETICA_18, "Navya Raina, 500106856");
    glFlush();
}
//void delay(float ms) {
//    clock_t goal = ms + clock();
//    while (goal > clock());
//}

void init2() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 400, 0, 600);
}
struct Point {
    GLint x;
    GLint y;
};

struct Color {
    float r;
    float g;
    float b;
};

Color getPixelColor(GLint x, GLint y) {
    Color color;
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &color);
    return color;
}
void setPixelColor(GLint x, GLint y, Color color) {
    glColor3f(color.r, color.g, color.b);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
    glFlush();
}

bool isSameColor(Color c1, Color c2, float tolerance = 0.01f) {
    return (fabs(c1.r - c2.r) < tolerance &&
        fabs(c1.g - c2.g) < tolerance &&
        fabs(c1.b - c2.b) < tolerance);
}

void floodFill(GLint x, GLint y, Color oldColor, Color newColor) {
    if (isSameColor(oldColor, newColor)) {
        return;
    }

    stack<Point> pixelStack;
    pixelStack.push({ x, y });

    while (!pixelStack.empty()) {
        Point p = pixelStack.top();
        pixelStack.pop();

        Color currentColor = getPixelColor(p.x, p.y);

        if (isSameColor(currentColor, oldColor)) {
            setPixelColor(p.x, p.y, newColor);

            if (p.x + 1 < 800) pixelStack.push({ p.x + 1, p.y });
            if (p.x - 1 >= 0) pixelStack.push({ p.x - 1, p.y });
            if (p.y + 1 < 600) pixelStack.push({ p.x, p.y + 1 });
            if (p.y - 1 >= 0) pixelStack.push({ p.x, p.y - 1 });
        }
    }
}
void mousefloodfill(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        flag = true;
        x2 = x;
        y2 = 600 - y;

    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        Color newColor = { 1.0f, 0.0f, 0.0f };
        Color oldColor = { 0.0f, 0.0f, 0.0f };

        floodFill(x2, y2, oldColor, newColor);
    }
    glutPostRedisplay();

}

void draw_continuous_circle(Point pC, GLfloat radius) {
    glLineWidth(3.0);
    glColor3f(0, 0, 0);

    glBegin(GL_LINE_LOOP);
    for (float angle = 0; angle <= 2 * M_PI; angle += 0.1) {
        GLfloat x = pC.x + (radius * cos(angle));
        GLfloat y = pC.y + (radius * sin(angle));
        glVertex2f(x, y);
    }
    glEnd();
    glFlush();
}

void display(void) {
    Point pt = { x2,y2 };
    GLfloat radius = 10;

    glClear(GL_COLOR_BUFFER_BIT);
    draw_continuous_circle(pt, radius);
    glColor3f(0, 0, 0);


    glFlush();
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(0,0);
    int mainWindow = glutCreateWindow("Boundary fill Algorithm on Left and Flood fill Algorithm on Right");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mousefloodfill);
    //glutFullScreen();

    int subWindow1 = glutCreateSubWindow(mainWindow, 0, 0, 400, 600);
    glutInitWindowPosition(100, 100);
    glutDisplayFunc(world);
    glutMouseFunc(mouseboundaryfill);
    init2();

    glutMainLoop();
    return 0;

}