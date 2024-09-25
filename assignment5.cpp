#include <GL/glut.h>
#include <iostream>
#include <cmath>

using namespace std;

int drawingMode = -1; 
int clickCount = 0; 

float points[3][2];

void plotPixel(float x, float y) {
   glBegin(GL_POINTS);
   glVertex2f(x, y);
   glEnd();
}

void drawAxes() {
   glColor3f(1, 1, 1);
   glBegin(GL_LINES);
   // X-axis
   glVertex2f(-1.0f, 0.0f);
   glVertex2f(1.0f, 0.0f);
   // Y-axis
   glVertex2f(0.0f, -1.0f);
   glVertex2f(0.0f, 1.0f);
   glEnd();
}

void drawDDALine(float x0, float y0, float x1, float y1) {
   /*float dx = x1 - x0;
   float dy = y1 - y0;
   float steps = max(abs(dx), abs(dy));
   float xInc = dx / steps;
   float yInc = dy / steps;

   float x = x0, y = y0;
   for (int i = 0; i <= steps; i++) {
       plotPixel(x, y);
       x += xInc;
       y += yInc;
   }*/
   if (x0 > x1) {
       swap(x0, x1);
       swap(y0, y1);
   }

   float delX = x1 - x0;
   float delY = y1 - y0;


   if (delX == 0 && y0 > y1) {
       swap(x0, x1);
       swap(y0, y1);
   }


   float maxStep = max(abs(delX), abs(delY));
   float xIncrement = delX / maxStep * 0.001;
   float yIncrement = delY / maxStep * 0.001;

   float currentX = x0;
   float currentY = y0;

   plotPixel(currentX, currentY);

   while (currentX < x1 || (delX == 0 && currentY < y1)) {
       currentX += xIncrement;
       currentY += yIncrement;
       plotPixel(currentX, currentY);
   }
}

void drawBresenhamLine(int x0, int y0, int x1, int y1) {
   int dx = abs(x1 - x0);
   int dy = abs(y1 - y0);
   int sx = x0 < x1 ? 1 : -1;
   int sy = y0 < y1 ? 1 : -1;
   int err = dx - dy;

   while (true) {
       plotPixel((float)x0 / 100, (float)y0 / 100);
       if (x0 == x1 && y0 == y1) break;
       int e2 = 2 * err;
       if (e2 > -dy) {
           err -= dy;
           x0 += sx;
       }
       if (e2 < dx) {
           err += dx;
           y0 += sy;
       }
   }
}

void drawEllipse(int xc, int yc, int rx, int ry) {
   int x = 0, y = ry;
   int rx2 = rx * rx, ry2 = ry * ry;
   int tworx2 = 2 * rx2, twory2 = 2 * ry2;
   int px = 0, py = tworx2 * y;

   plotPixel((float)(xc + x) / 100, (float)(yc + y) / 100);
   plotPixel((float)(xc - x) / 100, (float)(yc + y) / 100);
   plotPixel((float)(xc + x) / 100, (float)(yc - y) / 100);
   plotPixel((float)(xc - x) / 100, (float)(yc - y) / 100);

   int p = round(ry2 - (rx2 * ry) + (0.25 * rx2));
   while (px < py) {
       x++;
       px += twory2;
       if (p < 0) {
           p += ry2 + px;
       }
       else {
           y--;
           py -= tworx2;
           p += ry2 + px - py;
       }
       plotPixel((float)(xc + x) / 100, (float)(yc + y) / 100);
       plotPixel((float)(xc - x) / 100, (float)(yc + y) / 100);
       plotPixel((float)(xc + x) / 100, (float)(yc - y) / 100);
       plotPixel((float)(xc - x) / 100, (float)(yc - y) / 100);
   }

   p = round(ry2 * (x + 0.5) * (x + 0.5) + rx2 * (y - 1) * (y - 1) - rx2 * ry2);
   while (y > 0) {
       y--;
       py -= tworx2;
       if (p > 0) {
           p += rx2 - py;
       }
       else {
           x++;
           px += twory2;
           p += rx2 - py + px;
       }
       plotPixel((float)(xc + x) / 100, (float)(yc + y) / 100);
       plotPixel((float)(xc - x) / 100, (float)(yc + y) / 100);
       plotPixel((float)(xc + x) / 100, (float)(yc - y) / 100);
       plotPixel((float)(xc - x) / 100, (float)(yc - y) / 100);
   }
}

void renderBitmapString(float x, float y, void* font, const string& str) {
   glRasterPos2f(x, y);
   glColor3f(0.0f, 0.0f, 0.1f);
   for (char c : str) {
       glutBitmapCharacter(font, c);
   }
}

void display() {
   glClear(GL_COLOR_BUFFER_BIT);

   drawAxes();

   glColor3f(0.8, 0.8, 0.8);
   glBegin(GL_QUADS);
   glVertex2f(-0.9, 0.9);
   glVertex2f(-0.5, 0.9);
   glVertex2f(-0.5, 0.8);
   glVertex2f(-0.9, 0.8);
   glEnd();

   glBegin(GL_QUADS);
   glVertex2f(-0.4, 0.9);
   glVertex2f(0.0, 0.9);
   glVertex2f(0.0, 0.8);
   glVertex2f(-0.4, 0.8);
   glEnd();

   glBegin(GL_QUADS);
   glVertex2f(0.1, 0.9);
   glVertex2f(0.5, 0.9);
   glVertex2f(0.5, 0.8);
   glVertex2f(0.1, 0.8);
   glEnd();

   glColor3f(0, 0, 0);
   glRasterPos2f(-0.8, 0.85);
   glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'D');
   glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'D');
   glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'A');

   glRasterPos2f(-0.3, 0.85);
   glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'B');
   glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'R');
   glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'E');
   glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'S');
   glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'E');
   glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'N');
   glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'H');
   glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'A');
   glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'M');

   glRasterPos2f(0.2, 0.85);
   glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'E');
   glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'L');
   glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'L');
   glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'I');
   glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'P');
   glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'S');
   glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'E');


   glColor3f(1, 1, 1);
   if (drawingMode == 0 && clickCount >= 2) {
       drawDDALine(points[0][0], points[0][1], points[1][0], points[1][1]);
   }
   else if (drawingMode == 1 && clickCount >= 2) {
       drawBresenhamLine(points[0][0] * 100, points[0][1] * 100, points[1][0] * 100, points[1][1] * 100);
   }
   if (drawingMode == 2 && clickCount >= 3) {
       int rx = abs(points[0][0] - points[2][0]) * 100; // x-radius from center
       int ry = abs(points[1][1] - points[2][1]) * 100; // y-radius from center
       drawEllipse(points[2][0] * 100, points[2][1] * 100, rx, ry);
   }


   renderBitmapString(0.6, 0.8, GLUT_BITMAP_HELVETICA_18, "Navya Raina, 500106856");

   glFlush();
}

void handleMouse(int button, int state, int x, int y) {
   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
       float mx = (float)x / glutGet(GLUT_WINDOW_WIDTH) * 2 - 1;
       float my = 1 - (float)y / glutGet(GLUT_WINDOW_HEIGHT) * 2;

       if (mx > -0.9 && mx < -0.5 && my > 0.8 && my < 0.9) {
           drawingMode = 0; // DDA
           clickCount = 0;
       }
       else if (mx > -0.4 && mx < 0.0 && my > 0.8 && my < 0.9) {
           drawingMode = 1; // Bresenham
           clickCount = 0;
       }
       else if (mx > 0.1 && mx < 0.5 && my > 0.8 && my < 0.9) {
       drawingMode = 2; // Ellipse
       clickCount = 0;
       }
       else if (drawingMode != -1 && clickCount < 3) {
       // Store the click position
       points[clickCount][0] = mx;
       points[clickCount][1] = my;
       clickCount++;
       }
   }
   glutPostRedisplay();
}

int main(int argc, char** argv) {
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(800, 600);
   glutCreateWindow("Different Drawing Algorithms");
   gluOrtho2D(-1, 1, -1, 1);

   glutDisplayFunc(display);
   glutMouseFunc(handleMouse);
   glutMainLoop();
   return 0;
}
