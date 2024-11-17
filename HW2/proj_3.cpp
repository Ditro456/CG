#include <GL/glut.h>
#include <cmath>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

float ballX = 0.0f, ballY = 1.0f;
float velocityY = 0.05f;
float gravity = -0.002f;
float elasticity = 0.8f;
bool wireframe = false;
bool isMoving = true;

float scaleX = 1.0f, scaleY = 1.0f;
bool isSquashing = false;

float cameraAngleX = 0.0f, cameraAngleY = 0.0f;

void drawGround() {
    const int segments = 100;
    const float radius = 5.0f;

    glColor3f(0.6f, 0.8f, 0.6f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 0.0f, 0.0f);

    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * M_PI * i / segments;
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        glVertex3f(x, 0.0f, z);
    }
    glEnd();
}

void drawBall() {
    glPushMatrix();
    glTranslatef(ballX, ballY, 0.0f);
    glScalef(scaleX, scaleY, 1.0f);

    glColor3f(0.7f, 0.9f, 1.0f);

    if (wireframe) {
        glutWireSphere(0.2, 20, 20);
    }
    else {
        glutSolidSphere(0.2, 20, 20);
    }
    glPopMatrix();
}

void update(int value) {
    if (isMoving) {
        ballY += velocityY;
        velocityY += gravity;

        if (ballY <= 0.2f) {
            ballY = 0.2f;
            velocityY = -velocityY * elasticity;
            isSquashing = true;

            if (fabs(velocityY) < 0.01f) {
                isMoving = false;
                velocityY = 0.0f;
            }
        }

        if (isSquashing) {
            scaleX = 1.2f + fabs(velocityY) * 5;
            scaleY = 0.8f - fabs(velocityY) * 2.5;
            if (scaleX <= 1.0f) {
                scaleX = 1.0f;
                scaleY = 1.0f;
                isSquashing = false;
            }
        }
        else {
            scaleX = 1.0f;
            scaleY = 1.0f + fabs(velocityY) * 3;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void handleKeypress(unsigned char key, int x, int y) {
    switch (key) {
    case 'w':
        wireframe = true;
        break;
    case 's':
        wireframe = false;
        break;
    case 'h':
        if (!isMoving) {
            velocityY = 0.05f;
            isMoving = true;
        }
        break;
    case '+':
        velocityY += 0.01f;
        break;
    case '-':
        velocityY -= 0.01f;
        break;
    case 'a':
        cameraAngleY -= 5.0f;
        break;
    case 'd':
        cameraAngleY += 5.0f;
        break;
    case 'q':
        cameraAngleX -= 5.0f;
        break;
    case 'e':
        cameraAngleX += 5.0f;
        break;
    }
    glutPostRedisplay();
}

void setup() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat lightPosition[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    GLfloat ballMaterial[] = { 0.7f, 0.9f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ballMaterial);
    glMaterialfv(GL_FRONT, GL_SPECULAR, ballMaterial);
    glMaterialf(GL_FRONT, GL_SHININESS, 50.0f);

    GLfloat groundMaterial[] = { 0.6f, 0.8f, 0.6f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, groundMaterial);
    glMaterialfv(GL_FRONT, GL_SPECULAR, groundMaterial);
    glMaterialf(GL_FRONT, GL_SHININESS, 50.0f);
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glRotatef(cameraAngleX, 1.0f, 0.0f, 0.0f);
    glRotatef(cameraAngleY, 0.0f, 1.0f, 0.0f);

    drawGround();
    drawBall();

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Bouncing Ball on Circular Ground");

    setup();

    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeypress);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}