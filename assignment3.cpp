#include <GL/glut.h>
#include <iostream>
#include <string>

using namespace std;

bool firstClickDetected = false;
bool secondClickDetected = false;
float initialX, initialY;
float finalX, finalY;
string firstClickCoordinates = "";
string secondClickCoordinates = "";
float slope_line;

void plotPixel(float xCoord, float yCoord) {
   cout << "Plot X: " << xCoord << " Plot Y: " << yCoord << "\n";
   glColor3f(1, 0, 1);
   glBegin(GL_POINTS);
   glVertex2f(xCoord, yCoord);
   glEnd();
}

void renderBitmapString(float x, float y, void* font, const string& str) {
   glRasterPos2f(x, y);
   glColor3f(0.0f, 0.0f, 0.1f);
   for (char c : str) {
       glutBitmapCharacter(font, c);
   }
}

void renderLine(float xStart, float yStart, float xEnd, float yEnd) {
   
   if (xStart > xEnd) {
       swap(xStart, xEnd);
       swap(yStart, yEnd);
   }

   float delX = xEnd - xStart;
   float delY = yEnd - yStart;

   slope_line = ((delY) / (delX));


   if (delX == 0 && yStart > yEnd) {
       swap(xStart, xEnd);
       swap(yStart, yEnd);
   }


   float maxStep = max(abs(delX), abs(delY));
   float xIncrement = delX / maxStep * 0.001;
   float yIncrement = delY / maxStep * 0.001;

   float currentX = xStart;
   float currentY = yStart;

   plotPixel(currentX, currentY);

   while (currentX < xEnd || (delX == 0 && currentY < yEnd)) {
       currentX += xIncrement;
       currentY += yIncrement;
       plotPixel(currentX, currentY);
   }

}

void handleMouseClick(int mouseButton, int buttonState, int mouseX, int mouseY) {
   if (buttonState == GLUT_DOWN) {
       int keyModifiers = glutGetModifiers();

       if (mouseButton == GLUT_LEFT_BUTTON ) {
           float xNormalized = 5 * (float)mouseX / (glutGet(GLUT_WINDOW_WIDTH));
           float yNormalized = -5 * (float)mouseY / (glutGet(GLUT_WINDOW_HEIGHT));

           if (!firstClickDetected) {
               initialX = xNormalized;
               initialY = yNormalized;
               firstClickCoordinates = "First Pos: (" + to_string(initialX) + ", " + to_string(initialY) + ")";
               firstClickDetected = true;
               secondClickDetected = false;
           }
           else if (firstClickDetected && !secondClickDetected) {
               finalX = xNormalized;
               finalY = yNormalized;
               secondClickCoordinates = "Second Pos: (" + to_string(finalX) + ", " + to_string(finalY) + ")";
               secondClickDetected = true;
           }

           
       }
       else if (mouseButton == GLUT_RIGHT_BUTTON) {
           firstClickDetected = false;
           secondClickDetected = false;
           firstClickCoordinates = "";
           secondClickCoordinates = "";
       }
   }
   glutPostRedisplay();
}

void displayScene() {
   // Set background color to black
   glClearColor(0,0,0,1);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glColor3f(1,1,1); 
   glBegin(GL_LINES);
   glVertex2f(-1, 0);
   glVertex2f(1, 0);
   glEnd();

   glBegin(GL_LINES);
   glVertex2f(0, -1);
   glVertex2f(0, 1);
   glEnd();

   if (firstClickDetected && secondClickDetected) {
       renderLine(initialX, initialY, finalX, finalY);
       renderBitmapString(0.5, -0.9, GLUT_BITMAP_HELVETICA_18, "m: "+to_string(slope_line));
   }

   glColor3f(1,1,1); // Set text color to white
   if (!firstClickCoordinates.empty()) {
       renderBitmapString(0.5, -0.7, GLUT_BITMAP_HELVETICA_18, firstClickCoordinates);
   }
   glColor3f(1, 1, 1);
   if (!secondClickCoordinates.empty()) {
       renderBitmapString(0.5, -0.8, GLUT_BITMAP_HELVETICA_18, secondClickCoordinates);
   }

   renderBitmapString(0.5, 0.8, GLUT_BITMAP_HELVETICA_18, "Navya Raina, 500106856");
   glFlush();
}

int main(int argc, char** argv) {
   glutInit(&argc, argv);
   glutCreateWindow("DDA ALGO");
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   gluOrtho2D(0, 0, 0, 0);
   glutInitWindowSize(1080, 1080);

   glEnable(GL_TEXTURE_2D);
   glEnable(GL_DEPTH_TEST);
   glutMouseFunc(handleMouseClick);
   glutDisplayFunc(displayScene);
   glutMainLoop();
   return 0;
}