#include <GL/glut.h>
#include <GL/GL.h>
#include <GL/GLU.h>
int FlatShaded = 0; int Wireframed = 0;
//Flatshading과 Wireframe을 토글링 하기 위한 변수. 토글 = 스위치 같은 개념

int ViewX = 0, ViewY = 0;
//마우스 움직임에 따른 시점을 바꾸기 위한 변수

float x = 0, y = 0, z = 0;

bool left_button = 0;

int rotate1 = 0;

void InitLight() {
    GLfloat mat_diffuse[] = { 0.5, 0.4, 0.3, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_ambient[] = { 0.5, 0.4, 0.3, 1.0 };
    GLfloat mat_shininess[] = { 15.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
    GLfloat light_position[] = { -3, 6, 3.0, 0.0 };
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}



void MouseClick(GLint Button, GLint State, GLint X, GLint Y) {
    if (Button == GLUT_LEFT_BUTTON && State == GLUT_DOWN) {
        left_button = 1; //마우스 왼쪽 버튼을 누름
    }
    else {
        left_button = 0;
    }
}


void Motion(GLint X, GLint Y) {
    if (left_button == 1)
    {
        ViewX = -X;
        ViewY = -Y;
    }

    glutPostRedisplay();
}

void PilotView(GLfloat roll, GLfloat pitch, GLfloat yaw) {
    glRotatef(-roll, 0.0, 0.0, 1.0);
    glRotatef(-pitch, 1.0, 0.0, 0.0);
    glRotatef(-yaw, 0.0, 1.0, 0.0);
    glTranslatef(-x, -y, -z);
}


void Keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'q': case'Q':case '\033':
        exit(0);
        break;
    case 's':
        if (FlatShaded) {
            FlatShaded = 0;
            glShadeModel(GL_SMOOTH);
        }
        else {
            FlatShaded = 1;
            glShadeModel(GL_FLAT);
        }
        glutPostRedisplay();
        break;
    case 'w':
        if (Wireframed) {
            Wireframed = 0;
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else {
            Wireframed = 1;
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        glutPostRedisplay();
        break;
    default:
        break;
    }
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0.0, 0.5, 0.0, 0.3, -0.3, -1.0, 0.0, 1.0, 0.0);
    PilotView(0.0, ViewY, ViewX);
    glPushMatrix();
    glTranslatef(0.4, 0.0, -0.4);
    glScalef(1.0, 4.0, 1.0);
    glColor3f(1.0, 0.0, 0.0);
    glutSolidCube(0.1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.4, 0.0, 0.4);
    glScalef(1.0, 4.0, 1.0);
    glColor3f(0.0, 1.0, 0.0);
    glutSolidCube(0.1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.4, 0.0, -0.4);
    glScalef(1.0, 4.0, 1.0);
    glColor3f(0.0, 0.0, 1.0);
    glutSolidCube(0.1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.4, 0.0, 0.4);
    glScalef(1.0, 4.0, 1.0);
    glColor3f(1.0, 1.0, 1.0);
    glutSolidCube(0.1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0, 0.2, 0.0);
    glScalef(10.0, 1.0, 10.0);
    glutSolidCube(0.1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0, 0.35, 0.0);
    glutSolidTorus(0.01, 0.08, 8, 30);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0, 0.35, 0.0);
    glRotatef(rotate1, 0.0, rotate1, rotate1);
    glutWireSphere(0.07, 10, 30);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0, 0.25, 0.0);
    glScalef(1.0, 0.34, 1.0);
    glColor3f(1.0, 1.0, 1.0);
    glutSolidCube(0.1);
    glPopMatrix();
    glFlush();
}

void Timer(int Value) {
    rotate1 = (rotate1 + 5) % 360;
    glutPostRedisplay();
    glutTimerFunc(50, Timer, 1);
}

void Reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("프로젝트");

    glClearColor(0.1, 0.6, 0.6, 0.0);
    InitLight();
    glutDisplayFunc(Display);
    glutKeyboardFunc(Keyboard);
    glutMouseFunc(MouseClick);
    glutMotionFunc(Motion);
    glutTimerFunc(50, Timer, 1);
    glutReshapeFunc(Reshape);
    glutMainLoop();
}