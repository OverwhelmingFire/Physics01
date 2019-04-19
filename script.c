#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#define arraySize 1000
#define electronsNumber 60

int timeSpan = 10;

double x = 0.6;
double y = 0.6;
double z = 0.6;
double k = 0.2;

int X1 = 0;
int X2 = 0;
int Y1 = 0;
int Y2 = 0;

double baseX = -5;
double baseY = 0.0;
double addX = 0.0;
double addY = 0.0;

double scaleX = 0.3;
double scaleY = 0.3;

double verticalMax = 3.0;
double horizontalMax = 9.0;

int globalCounter = 0;

int DRAW_ELECTRONS = 0;
int MESSAGE_DISABLED = 0;
int HELP_DISABLED = 1;

typedef struct {
    double previousFieldValue;
    int pointer;
    double x, y;
    double values[2*arraySize];
} Electron;

Electron NewElectron(double _x, double _y) {
    Electron e;
    e.previousFieldValue = 0;
    e.pointer=0;
    e.x=_x;
    e.y=_y;
    for (int i = 0; i < 2*arraySize; i++) {
        e.values[i] = 0;
    }
    return e;
}

typedef struct {
    int pointer;
    double x, y;
    double values[2*arraySize];
} Beam;


Beam NewBeam(double _x, double _y) {
    Beam b;
    b.pointer=0;
    b.x=_x;
    b.y=_y;
    for (int i = 0; i < 2*arraySize; i++) {
        b.values[i] = 0;
    }
    return b;
}


typedef struct {
    int pointer;
    double values[arraySize];
} EMMatrix;


EMMatrix NewEMMatrix() {
    EMMatrix m;
    m.pointer=0;
    for (int i = 0; i < arraySize; i++) {
        m.values[i] = 0.0;
    }
    return m;
}

Beam b;
EMMatrix m;
Electron * arrayOfElectrons;



void addBeamMatrix(Beam *b, EMMatrix *em) {
    int width = sizeof((*em).values)/sizeof(double);
    int start = (int)((*b).x*100);
    for (int i = 0; i < width; i++) {
        (*em).values[i]=0;
    }
    for (int i = 0; i+start < width && i <= (*b).pointer; i++) {
        (*em).values[start+i]=(*b).values[(*b).pointer-i];
    }
}

void addElectronMatrix(Electron *e, EMMatrix *em) {
    int width = sizeof((*em).values)/sizeof(double);
    int start = (int)((*e).x*100);

    for (int i = 1; i+start < width && i < (*e).pointer; i++) {
        (*em).values[start+i]+=(*e).values[(*e).pointer-i];
    }

    for (int i = start-1; i >= 0 && i < (*e).pointer; i--) {
        (*em).values[i]+=(*e).values[(*e).pointer-(start-i)];
    }
}

void addElectronMatrixX(Electron *e, EMMatrix *em) {
    int width = sizeof((*em).values)/sizeof(double);
    int start = (int)((*e).x*100);

    for (int i = 1; i+start < width && i < (*e).pointer; i++) {
        (*em).values[start+i]+=(*e).values[(*e).pointer-i];
    }

    for (int i = start-1; i >= 0 && i < (*e).pointer; i--) {
        (*em).values[i]+=(*e).values[(*e).pointer-(start-i)];
    }
}

void drawBeam(Beam * b) {
    glBegin(GL_LINE_STRIP);
    glColor3f(0.9, 0.9, 0.0);
    for (int i = 0; i < arraySize && i < (*b).pointer; i++){
        glVertex3d((*b).x+(double)(i+1)/100, (*b).values[(*b).pointer-i-1], 0.0);
    }
    glEnd();
}
void drawElectron(Electron * e) {
    glBegin(GL_LINE_STRIP);
    glColor3f(0.2, 0.8, 0.8);
    for (int i = 0; i < 2*arraySize && i < (*e).pointer; i++){
        glVertex3d((*e).x-(double)((*e).pointer-i-1)/100, 100*(*e).values[i], 0.0);
    }
    for (int i = 0; i < 2*arraySize && i < (*e).pointer; i++){
        glVertex3d((*e).x+(double)i/100, 100*((*e).values[(*e).pointer-i-1]), 0.0);
        if ((*e).values[i]==0 && (*e).values[i-1]==0) {
            break;
        }
    }
    glEnd();
}

void beamPropagate(Beam *b) {
    if ((*b).pointer>=2*arraySize) {
        for (int i = 0; i < arraySize; i++) {
            (*b).values[i]=(*b).values[i+arraySize];
        }
        for (int i = arraySize; i < 2*arraySize; i++) {
            (*b).values[i]=0;
        }
        (*b).pointer=arraySize;
    }

    (*b).values[(*b).pointer]=sin(globalCounter*M_PI/180);
    (*b).pointer++;
}

void electronPropagateX(Electron *e, double field) {


    if ((*e).pointer>=2*arraySize) {
        for (int i = 0; i < arraySize; i++) {
            (*e).values[i]=(*e).values[i+arraySize];
        }
        for (int i = arraySize; i < 2*arraySize; i++) {
            (*e).values[i]=0;
        }
        (*e).pointer=arraySize;
    }
    (*e).values[(*e).pointer]=-k*(field-(*e).previousFieldValue);
    (*e).previousFieldValue=field;
    (*e).pointer++;
}

void electronPropagate(Electron *e, double field) {
    if ((*e).pointer>=2*arraySize) {
        for (int i = 0; i < arraySize; i++) {
            (*e).values[i]=(*e).values[i+arraySize];
        }
        for (int i = arraySize; i < 2*arraySize; i++) {
            (*e).values[i]=0;
        }
        (*e).pointer=arraySize;
    }
    (*e).values[(*e).pointer]=-k*(field-(*e).previousFieldValue);
    (*e).previousFieldValue=field;
    (*e).pointer++;
}

void renderString(float x, float y, void *font, const char* string)
{
  char *c;
  glBegin(GL_POINTS);
  glColor3f(0.0,0.0,0.0);
  glVertex3d(0,0,0);
  glEnd();
  glRasterPos2f(x, y);
  glutBitmapString(font, string);
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    // We create the first matrix to display the waves.
    glLoadIdentity();
    glScalef(scaleX,scaleY,0.3);
    glTranslatef(baseX+addX, baseY+addY, 0.0);

    glBegin(GL_POLYGON);
    glColor3f(0.0,0.0,0.0);
    glVertex3d(-horizontalMax,1,0);
    glVertex3d(b.x,1,0);
    glVertex3d(b.x,-1,0);
    glVertex3d(-horizontalMax,-1,0);
    glEnd();


    beamPropagate(&b);
    for (int i = 0; i < electronsNumber; i++) {
        electronPropagate(&(arrayOfElectrons[i]), m.values[(int)(arrayOfElectrons[i].x*100)]);
    }

    addBeamMatrix(&b, &m);

    for (int i = 0; i < electronsNumber; i++) {
        addElectronMatrix(&(arrayOfElectrons[i]), &m);
    }

    globalCounter++;

    glBegin(GL_POINTS);
    glColor3f(0.0,0.0,0.9);
    for (int i = 0; i < electronsNumber; i++) {
            for (int k = 0; k < 100; k++) {
                glVertex3d(arrayOfElectrons[i].x, arrayOfElectrons[i].y+(double)k/10, 0.0);

                glVertex3d(arrayOfElectrons[i].x, -arrayOfElectrons[i].y-(double)k/10, 0.0);
            }
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    glColor3f(0.5, 0.3, 0.2);
    for (int i = 0; i < arraySize; i++){
        glVertex3d((double)i/100, m.values[i], 0.0);
    }
    glEnd();

    drawBeam(&b);
    if (DRAW_ELECTRONS == 1) {
        for (int i = 0; i < electronsNumber; i++) {
            drawElectron(&(arrayOfElectrons[i]));
        }
    }

    // Now we create the second matrix to display text.
    glLoadIdentity();

    if (MESSAGE_DISABLED == 0) {
        renderString(-0.9f, 0.9f, GLUT_BITMAP_HELVETICA_18, "Yellow color stands for the original \nbeam from the source.\n\nBlue color stands for the electromagnetic waves \ngenerated by electrons.\n\nBlack color stands for the resulting wave.\n\nYou can disable this message by pressing 'q'.");
    }
    if (HELP_DISABLED == 0) {
        renderString(-0.9f, 0.9f, GLUT_BITMAP_HELVETICA_18, "e - show waves produced by electrons \narrow up - slow down animation \narrow down - speed up animation \nc - clear window and reset animation");
    }
    glFlush();
    glutSwapBuffers();
}

void update(int value)
{
        glutPostRedisplay();
        glutTimerFunc(timeSpan, update, 0);
}

void initRendering()
{
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_COLOR_MATERIAL);

        glClearColor(0.9f, 0.9f, 1.0f, 1.0f);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_NORMALIZE);
}

void handleResize(int w, int h)
{
        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
}

void mouse(int button, int state, int x, int y)
{

    switch (button)
    {
        case GLUT_LEFT_BUTTON:
            {
                if(state == GLUT_DOWN)
                {
                    X1=x;
                    Y1=y;
                }
                else
                {
                    X2=x;
                    Y2=y;
                    baseX+=addX;
                    baseY+=addY;
                    addX = 0;
                    addY = 0;
                }
            break;
        }

        case 3:
            {
                if(state == GLUT_UP) {
                    scaleX += 0.05;
                    scaleY += 0.05;
                }
                else {
                    scaleX -= 0.05;
                    scaleY -= 0.05;
                }
            break;
        }

        case 4:
            {
                if(state == GLUT_UP) {
                    scaleX += 0.05;
                    scaleY += 0.05;
                }
                else {
                    scaleX -= 0.05;
                    scaleY -= 0.05;
                }
            break;
        }
    }
}


void keyboard(unsigned char key, int x, int y) {
    switch (key)
    {
        case 'e':
            {
                if(DRAW_ELECTRONS == 1) {
                    DRAW_ELECTRONS = 0;
                }
                else {
                    DRAW_ELECTRONS = 1;
                }
            break;
        }
        case 'c':
        {
            for (int i = 0; i < 2*arraySize; i++) {
                b.values[i]=0;
            }
            for (int i = 0; i < arraySize; i++) {
                m.values[i]=0;
            }
            for (int k = 0; k < electronsNumber; k++) {
                for (int i = 0; i < 2*arraySize; i++) {
                    (arrayOfElectrons[k]).values[i]=0;
                }
                (arrayOfElectrons[k]).previousFieldValue=0;
            }
            globalCounter=0;
            break;
        }
        case 'q':
            {
                if(MESSAGE_DISABLED == 1) {
                    MESSAGE_DISABLED = 0;
                    if (HELP_DISABLED == 0) HELP_DISABLED = 1;
                }
                else {
                    MESSAGE_DISABLED = 1;
                }
            break;
        }
        case 'h':
            {
                if(HELP_DISABLED == 1) {
                    HELP_DISABLED = 0;
                    if (MESSAGE_DISABLED == 0) MESSAGE_DISABLED = 1;
                }
                else {
                    HELP_DISABLED = 1;
                }
            break;
        }
    }
}

void special(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            {
                timeSpan++;
            break;
        }
        case GLUT_KEY_DOWN:
            {
                if (timeSpan > 0) {
                    timeSpan--;
                }
            break;
        }
    }
}

void mouseMoving(int x, int y) {
    addX=(double)(x-X1)/((double)glutGet(GLUT_WINDOW_WIDTH)*scaleX*0.5);
    addY=(double)(Y1-y)/((double)glutGet(GLUT_WINDOW_HEIGHT)*scaleY*0.5);
    if (addX+baseX>0) {
        addX = 0-baseX;
    }
    else if (baseX+addX<-horizontalMax) {
        addX = -(baseX+horizontalMax);
    }
    if (addY+baseY>verticalMax) {
        addY = verticalMax-baseY;
    }
    else if (baseY+addY<-verticalMax) {
        addY = -(verticalMax+baseY);
    }
}

int main(int argc, char **argv)
{
    b = NewBeam(0.0, 0);
    m = NewEMMatrix();
    arrayOfElectrons = malloc(electronsNumber*sizeof(Electron));
    for (int i = 0; i < electronsNumber; i++){
        arrayOfElectrons[i] = NewElectron(0.7+(double)i/10,0);
    }
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Electromangetic Wave simulator");
    initRendering();

    glutDisplayFunc(draw);
    glutReshapeFunc(handleResize);
    glutSpecialFunc(special);
    glutTimerFunc(25, update, 0);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMotionFunc(mouseMoving);

    glutMainLoop();
    return 0;
}