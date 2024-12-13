#include <GL/freeglut.h>
#define _USE_MATH_DEFINES
#include <cmath>

// ī�޶� ��ġ�� ����
float cameraX = 0.0f, cameraY = 2.0f, cameraZ = 10.0f;
float cameraAngleX = 0.0f, cameraAngleY = 0.0f;

// Ű �Է¿� ���� ī�޶� �̵�
void handleKeys(unsigned char key, int x, int y) {
    const float moveSpeed = 0.5f;
    const float rotationSpeed = 2.0f;

    switch (key) {
    case 'w': cameraZ -= moveSpeed; break; // ������ �̵�
    case 's': cameraZ += moveSpeed; break; // �ڷ� �̵�
    case 'a': cameraX -= moveSpeed; break; // ���� �̵�
    case 'd': cameraX += moveSpeed; break; // ������ �̵�
    case 'q': cameraY -= moveSpeed; break; // �Ʒ��� �̵�
    case 'e': cameraY += moveSpeed; break; // ���� �̵�
    case 'j': cameraAngleY -= rotationSpeed; break; // ���� ȸ��
    case 'l': cameraAngleY += rotationSpeed; break; // ������ ȸ��
    case 'i': cameraAngleX -= rotationSpeed; break; // ���� ȸ��
    case 'k': cameraAngleX += rotationSpeed; break; // �Ʒ��� ȸ��
    case 27: exit(0); // ESC Ű�� ����
    }
    glutPostRedisplay();
}

// ���� ����
void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // ���� ��ġ
    GLfloat lightPos[] = { 5.0f, 10.0f, 10.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // ���� ����
    GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat lightDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
}

// ��ǰ �׸���
void drawComponents() {
    // Ĩ(ť��)
    glPushMatrix();
    glColor3f(0.2f, 0.2f, 0.8f);
    glTranslatef(0.0f, 0.5f, 0.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // �ܵ���(�����)
    glPushMatrix();
    glColor3f(0.8f, 0.2f, 0.2f);
    glTranslatef(2.0f, 0.5f, 0.0f);
    glRotatef(90, 1.0f, 0.0f, 0.0f);
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, 0.2, 0.2, 1.0, 32, 32);
    gluDeleteQuadric(quad);
    glPopMatrix();

    // ����(������ü)
    glPushMatrix();
    glColor3f(0.6f, 0.4f, 0.2f);
    glTranslatef(-2.0f, 0.5f, 0.0f);
    glScalef(2.0f, 0.5f, 0.5f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

// ���÷��� �ݹ�
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // ī�޶� ����
    gluLookAt(cameraX, cameraY, cameraZ,
        cameraX + sin(cameraAngleY * M_PI / 180.0),
        cameraY + sin(cameraAngleX * M_PI / 180.0),
        cameraZ - cos(cameraAngleY * M_PI / 180.0),
        0.0, 1.0, 0.0);

    // PCB �ٴ�
    glPushMatrix();
    glColor3f(0.2f, 0.6f, 0.2f);
    glTranslatef(0.0f, -0.5f, 0.0f);
    glScalef(10.0f, 0.1f, 10.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // ���� ��ǰ
    drawComponents();

    glutSwapBuffers();
}

// �ʱ�ȭ
void init() {
    glEnable(GL_DEPTH_TEST); // ���� �׽�Ʈ Ȱ��ȭ
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    setupLighting();
}

// ���� �Լ�
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D PCB Viewer");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeys);

    glutMainLoop();
    return 0;
}
