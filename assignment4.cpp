#include <GL/glut.h>
#include <iostream>
#include <string>

using namespace std;

struct Point {
   float x, y;
};

Point firstPoint = { -1, -1 };
Point secondPoint = { -1, -1 };
bool firstClickDetected = false;
bool secondClickDetected = false;
float slope_line;
string firstClickCoordinates = "";
string secondClickCoordinates = "";

// Function to draw the coordinate axes
void drawAxes() {
   glColor3f(1.0, 1.0, 1.0); // Set color to white
   glBegin(GL_LINES);
   // Draw X-axis
   glVertex2f(-10.0, 0.0);
   glVertex2f(10.0, 0.0);
   // Draw Y-axis
   glVertex2f(0.0, -10.0);
   glVertex2f(0.0, 10.0);
   glEnd();
}

void renderBitmapString(float x, float y, void* font, const string& str) {
   glRasterPos2f(x, y);
   glColor3f(0.0f, 0.0f, 0.1f);
   for (char c : str) {
       glutBitmapCharacter(font, c);
   }
}

// Bresenham's line drawing algorithm
void drawLine(Point p1, Point p2) {
   int x1 = static_cast<int>(p1.x * 100);  // Scale to match the integer calculations
   int y1 = static_cast<int>(p1.y * 100);
   int x2 = static_cast<int>(p2.x * 100);
   int y2 = static_cast<int>(p2.y * 100);

   slope_line = ((y2 - y1) / (x2 - x1));

   int dx = abs(x2 - x1);
   int dy = abs(y2 - y1);

   int sx = (x1 < x2) ? 1 : -1;
   int sy = (y1 < y2) ? 1 : -1;
   int err = dx - dy;

   glBegin(GL_POINTS);
   while (true) {
       // Draw the point, mapping the integer coordinates back to float space
       glVertex2f((float)x1 / 100, (float)y1 / 100);

       if (x1 == x2 && y1 == y2)
           break;

       int e2 = 2 * err;
       if (e2 > -dy) {
           err -= dy;
           x1 += sx;
       }
       if (e2 < dx) {
           err += dx;
           y1 += sy;
       }
   }
   glEnd();
   glFlush();
}

// Mouse callback function
void mouse(int button, int state, int x, int y) {
   if (state == GLUT_DOWN) {
       // Transform screen coordinates to OpenGL coordinates within -10 to 10 range
       float transformedX = (x / 50.0f) - 10.0f;
       float transformedY = 10.0f - (y / 50.0f);

       if (button == GLUT_LEFT_BUTTON) {
           if (!firstClickDetected) {
               firstPoint = { transformedX,transformedY };
               firstClickCoordinates = "First Pos: (" + to_string(transformedX) + ", " + to_string(transformedY) + ")";
               firstClickDetected = true;
               secondClickDetected = false;
           }
           else if (firstClickDetected && !secondClickDetected) {
               secondPoint = { transformedX, transformedY };
               secondClickCoordinates = "Second Pos: (" + to_string(transformedX) + ", " + to_string(transformedY) + ")";
               secondClickDetected = true;
           }

       }
       else if (button == GLUT_RIGHT_BUTTON) {
           firstClickDetected = false;
           secondClickDetected = false;
                   
       }
   }
   glutPostRedisplay();
}

// Display function
void display() {
   glClear(GL_COLOR_BUFFER_BIT);
   drawAxes();

   if (firstClickDetected && secondClickDetected) {
       drawLine(firstPoint, secondPoint);
       renderBitmapString(8,5, GLUT_BITMAP_HELVETICA_18, "m: " + to_string(slope_line));
   }
   if (!firstClickCoordinates.empty()) {
       renderBitmapString(-7,6, GLUT_BITMAP_HELVETICA_18, firstClickCoordinates);
   }
   glColor3f(1, 1, 1);
   if (!secondClickCoordinates.empty()) {
       renderBitmapString(-7, 8, GLUT_BITMAP_HELVETICA_18, secondClickCoordinates);
   }

   renderBitmapString(-9,8, GLUT_BITMAP_HELVETICA_18, "Navya Raina, 500106856");

   glFlush();
}

// Main function
int main(int argc, char** argv) {
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(1000, 1000); // Window size
   glutInitWindowPosition(100, 100);
   glutCreateWindow("Bresenham's Line Drawing with Quadrants");

   // Set up the orthographic projection with a coordinate range of -10 to 10
   gluOrtho2D(-10, 10, -10, 10);

   glutDisplayFunc(display);
   glutMouseFunc(mouse);
   glClearColor(0.0, 0.0, 0.0, 0.0); // Black background
   glutMainLoop();
   return 0;
}
