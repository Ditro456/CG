#include <GL/freeglut.h>
#define _USE_MATH_DEFINES
#include <cmath>

// 카메라 위치와 방향
float cameraX = 0.0f, cameraY = 2.0f, cameraZ = 10.0f;
float cameraAngleX = 0.0f, cameraAngleY = 0.0f;

// 키 입력에 따라 카메라 이동
void handleKeys(unsigned char key, int x, int y) {
    const float moveSpeed = 0.5f;
    const float rotationSpeed = 2.0f;

    switch (key) {
    case 'w': cameraZ -= moveSpeed; break; // 앞으로 이동
    case 's': cameraZ += moveSpeed; break; // 뒤로 이동
    case 'a': cameraX -= moveSpeed; break; // 왼쪽 이동
    case 'd': cameraX += moveSpeed; break; // 오른쪽 이동
    case 'q': cameraY -= moveSpeed; break; // 아래로 이동
    case 'e': cameraY += moveSpeed; break; // 위로 이동
    case 'j': cameraAngleY -= rotationSpeed; break; // 왼쪽 회전
    case 'l': cameraAngleY += rotationSpeed; break; // 오른쪽 회전
    case 'i': cameraAngleX -= rotationSpeed; break; // 위로 회전
    case 'k': cameraAngleX += rotationSpeed; break; // 아래로 회전
    case 27: exit(0); // ESC 키로 종료
    }
    glutPostRedisplay();
}

// 조명 설정
void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // 조명 위치
    GLfloat lightPos[] = { 5.0f, 10.0f, 10.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // 조명 색상
    GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat lightDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
}

// 부품 그리기
void drawComponents() {
    // 칩(큐브)
    glPushMatrix();
    glColor3f(0.2f, 0.2f, 0.8f);
    glTranslatef(0.0f, 0.5f, 0.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // 콘덴서(원기둥)
    glPushMatrix();
    glColor3f(0.8f, 0.2f, 0.2f);
    glTranslatef(2.0f, 0.5f, 0.0f);
    glRotatef(90, 1.0f, 0.0f, 0.0f);
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, 0.2, 0.2, 1.0, 32, 32);
    gluDeleteQuadric(quad);
    glPopMatrix();

    // 저항(직육면체)
    glPushMatrix();
    glColor3f(0.6f, 0.4f, 0.2f);
    glTranslatef(-2.0f, 0.5f, 0.0f);
    glScalef(2.0f, 0.5f, 0.5f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

// 디스플레이 콜백
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 카메라 설정
    gluLookAt(cameraX, cameraY, cameraZ,
        cameraX + sin(cameraAngleY * M_PI / 180.0),
        cameraY + sin(cameraAngleX * M_PI / 180.0),
        cameraZ - cos(cameraAngleY * M_PI / 180.0),
        0.0, 1.0, 0.0);

    // PCB 바닥
    glPushMatrix();
    glColor3f(0.2f, 0.6f, 0.2f);
    glTranslatef(0.0f, -0.5f, 0.0f);
    glScalef(10.0f, 0.1f, 10.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // 전자 부품
    drawComponents();

    glutSwapBuffers();
}

// 초기화
void init() {
    glEnable(GL_DEPTH_TEST); // 깊이 테스트 활성화
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    setupLighting();
}

// 메인 함수
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
