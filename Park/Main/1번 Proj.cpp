#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <iostream>

float playerX = 0.0f, playerY = -0.5f, playerZ = 0.0f;
float step = 0.02f;
bool walking = false;
float legAngle = 0.0f;
float legSpeed = 5.0f;
float timeOfDay = 0.0f;
float timeStep = 0.001f;

struct Object {
    float x, y, z;
};

std::vector<Object> trees = {
    {-0.8f, 0.0f, -5.0f}, {-0.6f, 0.0f, -5.0f}, {0.6f, 0.0f, -6.0f}, {0.8f, 0.0f, -7.0f}, {0.9f, 0.0f, -8.0f}, {-0.7f, 0.0f, -9.0f}, {0.5f, 0.0f, -10.0f}
};

std::vector<Object> signboards = {
    {-0.5f, 0.0f, -5.0f}, {0.5f, 0.0f, -6.0f}, {-0.7f, 0.0f, -7.0f}, {0.7f, 0.0f, -8.0f}
};


void displayHelp() {
    std::cout << "Help: \n";
    std::cout << "  W: Start walking \n";
    std::cout << "  Space: Stop walking \n";
    std::cout << "  H: Show help \n";
    std::cout << "  Q: Quit the application \n";
}

void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat lightDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat lightSpecular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat lightPosition[] = { 1.0f, 1.0f, 1.0f, 0.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glEnable(GL_COLOR_MATERIAL);

    GLfloat materialAmbientDiffuse[] = { 0.2f, 0.7f, 0.9f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialAmbientDiffuse);



    GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, 50.0f);
}

void drawPlayer() {
    glPushMatrix();
    glTranslatef(playerX, playerY, playerZ);

    glColor3f(0.3f, 0.7f, 0.9f);
    glPushMatrix();
    glScalef(0.1f, 0.3f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glColor3f(1.0f, 0.8f, 0.6f);
    glPushMatrix();
    glTranslatef(0.0f, 0.25f, 0.0f);
    glutSolidSphere(0.05f, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.03f, -0.2f, 0.0f);
    glRotatef(legAngle, 1.0f, 0.0f, 0.0f);
    glColor3f(0.1f, 0.1f, 0.1f);
    glPushMatrix();
    glScalef(0.02f, 0.2f, 0.02f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.03f, -0.2f, 0.0f);
    glRotatef(-legAngle, 1.0f, 0.0f, 0.0f);
    glColor3f(0.1f, 0.1f, 0.1f);
    glPushMatrix();
    glScalef(0.02f, 0.2f, 0.02f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPopMatrix();

    glPopMatrix();
}

// Draw trees
void drawTree(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);

    // Draw tree trunk
    
    glColor3f(0.5f, 0.3f, 0.1f);
    glPushMatrix();
    
    glScalef(0.05f, 0.2f, 0.05f);
    
    glutSolidCube(1.0f);
    glColor3f(0.5f, 0.3f, 0.1f);
    glPopMatrix();

    // Draw foliage
    glColor3f(0.0f, 0.6f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f, 0.15f, 0.0f);
    glutSolidSphere(0.1f, 20, 20);
    glPopMatrix();

    glPopMatrix();
}

// Draw signboards
void drawSignboard(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);

    // Signboard pole
    glColor3f(0.4f, 0.2f, 0.1f);
    glPushMatrix();
    glScalef(0.02f, 0.3f, 0.02f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Signboard
    glColor3f(0.9f, 0.9f, 0.9f);
    glPushMatrix();
    glTranslatef(0.0f, 0.2f, 0.0f);
    glScalef(0.2f, 0.1f, 0.05f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Add text to the signboard
    glPushMatrix();
    glTranslatef(0.0f, 0.22f, 0.03f);
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos3f(-0.06f, 0.0f, 0.02f); // Adjusted for better visibility
    std::string text = "Warning!";
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
    glPopMatrix();

    glPopMatrix();
}

// Draw the pathway
void drawPathway() {
    glColor3f(0.6f, 0.5f, 0.4f);
    glBegin(GL_QUADS);
    glVertex3f(-0.3f, -0.5f, -50.0f);
    glVertex3f(0.3f, -0.5f, -50.0f);
    glVertex3f(0.3f, -0.5f, 0.0f);
    glVertex3f(-0.3f, -0.5f, 0.0f);
    glEnd();
}

// update 캐릭터, 나무, 간판, 석양
void update(int value) {
    if (walking) {

        legAngle += legSpeed;
        if (legAngle > 30.0f || legAngle < -30.0f) {
            legSpeed = -legSpeed;
        }

        for (auto& tree : trees) {
            tree.z += step;
            if (tree.z > 0.0f) {
                tree.z = -10.0f;
            }
        }

        for (auto& signboard : signboards) {
            signboard.z += step;
            if (signboard.z > 0.0f) {
                signboard.z = -10.0f;
            }
        }
    }

    timeOfDay += timeStep;
    if (timeOfDay > 1.0f) timeOfDay = 0.0f;


    float morningRed = 0.5f, morningGreen = 0.8f, morningBlue = 1.0f;
    float sunsetRed = 1.0f, sunsetGreen = 0.5f, sunsetBlue = 0.3f;

    float r = (1.0f - timeOfDay) * morningRed + timeOfDay * sunsetRed;
    float g = (1.0f - timeOfDay) * morningGreen + timeOfDay * sunsetGreen;
    float b = (1.0f - timeOfDay) * morningBlue + timeOfDay * sunsetBlue;

    glClearColor(r, g, b, 1.0f);

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}


void handleKeys(unsigned char key, int x, int y) {
    if (key == 'w') {
        walking = true;
    }
    else if (key == ' ') {
        walking = false;
    }
    else if (key == 'h' || key == 'H') {
        displayHelp();
    }
    else if (key == 'q' || key == 'Q') {
        exit(0);
    }
}

// 렌더 씬 함수
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //카메라 위치, 방향, 벡터
    gluLookAt(0.0f, 0.5f, 2.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 0.0f);

    drawPathway();

    for (const auto& tree : trees) {
        drawTree(tree.x, tree.y, tree.z);
    }

    for (const auto& signboard : signboards) {
        drawSignboard(signboard.x, signboard.y, signboard.z);
    }

    drawPlayer();

    glutSwapBuffers();
}

//초기화
void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    setupLighting();
    glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 1.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
}

//메인 함수
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Park Walking");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeys);
    glutTimerFunc(16, update, 0);
    glutMainLoop();
    return 0;
}
