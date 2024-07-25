#define GL_SILENCE_DEPRECATION

#include <GL/glut.h>
// #include "game.h"
#include <math.h>

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 800

#define VIEW_SIZE 10
#define BORDER_SIZE 0.25

/* game.c */

#include "game.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

static int *randomPermutation()
{
    int *currentNums = (int *)calloc(NUM_BOXES, sizeof(int));
    if (!currentNums)
    {
        free(currentNums);
        return NULL;
    }
    for (int i = 0; i < NUM_BOXES; i++)
    {
        currentNums[i] = nums[i];
    }
    srand(time(NULL));
    for (int i = NUM_BOXES - 1; i > 0; i--)
    {
        int j = (int)rand() % (i + 1);
        arraySwap(&currentNums[i], &currentNums[j]);
    }
    return currentNums;
}

static void arraySwap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Groups are either 1,2, or 3. 0 is denoted as no group yet.
static group_t *generateGroups()
{
    group_t *currentGroups = (group_t *)calloc(NUM_BOXES, sizeof(group_t));
    if (!currentGroups)
        return NULL;
    // First Group
    currentGroups[0] = RED;
    int index = 0;
    if ((int)rand() % 2 == 0) // Right
        index = 1;
    else // Up
        index = 3;
    currentGroups[index] = RED;
    // if ((int)rand() % 2 == 0)     //Another block?
    if ((int)rand() % 2 == 0) // Right
        index += 1;
    else // Up
        index += 3;
    currentGroups[index] = RED;
    if ((int)rand() % 3 == 0)                       // Another block?
        if ((int)rand() % 2 == 0 && index % 3 != 2) // Right
            index += 1;
        else if (index < 6) // Up
            index += 3;
    currentGroups[index] = RED;
    // Secound Group
    index = 8;
    currentGroups[index] = GREEN;
    if (currentGroups[5] == RED)
        index = 7;
    else if (currentGroups[7] == RED)
        index = 5;
    else if ((int)rand() % 2 == 0) // Left
        index = 7;
    else // Down
        index = 5;
    currentGroups[index] = GREEN;
    if ((int)rand() % 3 == 0)                                      // Another block?
        if ((int)rand() % 2 == 0 && currentGroups[index - 1] == 0) // Left
            index -= 1;
        else if (currentGroups[index - 3] == 0) // Down
            index -= 3;
    currentGroups[index] = GREEN;

    // Third Group
    for (index = 0; index < 9; index++)
        if (currentGroups[index] == WHITE)
            currentGroups[index] = BLUE;
    return currentGroups;
}

void generateBoard()
{
    int *numList = randomPermutation();
    group_t *groupList = generateGroups();
    if (!numList)
        return;
    if (!groupList)
        return;
    for (int i = 0; i < NUM_BOXES; i++)
    {
        sukoBlocks[i] = (block_t){numList[i], i, groupList[i]};
    }
}

/* end game.c*/

int boolReveal = 0;

void drawFullCircle(double cx, double cy, double r)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (double angle = 0; angle < 2 * M_PI + 0.01; angle += 0.001)
    {
        double x = cx + r * cos(angle);
        double y = cy + r * sin(angle);

        glVertex2f(x, y);
    }
    glEnd();
}

void drawEmptyCircle(double cx, double cy, double r)
{
    glBegin(GL_LINE_LOOP);
    for (double angle = 0; angle < 2 * M_PI; angle += 0.1)
    {
        double x = cx + r * cos(angle);
        double y = cy + r * sin(angle);

        glVertex2f(x, y);
    }
    glEnd();
}

void drawText(double x, double y, char *text, double scale)
{
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scale / 152.38, scale / 152.38, scale / 152.38);
    glColor3f(0, 0, 0);
    for (char *p = text; *p; p++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
    }
    glPopMatrix();
}

void drawBlock(block_t b)
{
    int col = b.position % 3;
    int row = (b.position - col) / 3;

    switch (b.group)
    {
    case RED:
        glColor3f(1, 0.3, 0.3);
        break;
    case GREEN:
        glColor3f(0.3, 1, 0.3);
        break;
    case BLUE:
        glColor3f(0.3, 0.3, 1);
        break;
    case WHITE:
        glColor3f(1, 1, 1);
        break;
    default:
        glColor3f(0, 1, 1);
        break;
    }

    glBegin(GL_POLYGON);
    int blockWidth = (VIEW_SIZE - 4 * BORDER_SIZE) / 3;
    glVertex2f((blockWidth + BORDER_SIZE) * col + BORDER_SIZE, (blockWidth + BORDER_SIZE) * row + BORDER_SIZE);
    glVertex2f((blockWidth + BORDER_SIZE) * col + (blockWidth + BORDER_SIZE), (blockWidth + BORDER_SIZE) * row + BORDER_SIZE);
    glVertex2f((blockWidth + BORDER_SIZE) * col + (blockWidth + BORDER_SIZE), (blockWidth + BORDER_SIZE) * row + (blockWidth + BORDER_SIZE));
    glVertex2f((blockWidth + BORDER_SIZE) * col + BORDER_SIZE, (blockWidth + BORDER_SIZE) * row + (blockWidth + BORDER_SIZE));
    glEnd();

    if (boolReveal == 1)
    {
        char character = b.value + '0';
        drawText((blockWidth + BORDER_SIZE) * col + blockWidth / 2.0, (blockWidth + BORDER_SIZE) * row + blockWidth / 2.0, &character, 1);
    }
}

void renderScene(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int i = 0; i < NUM_BOXES; i++)
        drawBlock(sukoBlocks[i]);

    // Sum of Adjacents
    int blockSize = (VIEW_SIZE - 4 * BORDER_SIZE) / 3;

    // 1,2,4,5
    float cx = blockSize + 3 * BORDER_SIZE / 2.0;
    float cy = cx;
    glColor3f(1, 1, 1);
    drawFullCircle(cx, cy, VIEW_SIZE / 13.0);
    glColor3f(0, 0, 0);
    drawEmptyCircle(cx, cy, VIEW_SIZE / 13.0);
    int value = sukoBlocks[0].value + sukoBlocks[1].value + sukoBlocks[3].value + sukoBlocks[4].value;
    char num[3];
    num[0] = value / 10 + '0';
    num[1] = value % 10 + '0';
    num[2] = '\0';
    drawText(cx - 0.5, cy - 0.3, num, 1);

    // 2,3,5,6
    cx = 2 * blockSize + 5 * BORDER_SIZE / 2.0;
    ;
    cy = blockSize + 3 * BORDER_SIZE / 2.0;
    glColor3f(1, 1, 1);
    drawFullCircle(cx, cy, VIEW_SIZE / 13.0);
    glColor3f(0, 0, 0);
    drawEmptyCircle(cx, cy, VIEW_SIZE / 13.0);
    value = sukoBlocks[1].value + sukoBlocks[2].value + sukoBlocks[4].value + sukoBlocks[5].value;
    num[0] = value / 10 + '0';
    num[1] = value % 10 + '0';
    drawText(cx - 0.5, cy - 0.3, num, 1);

    // 4,5,7,8
    cx = blockSize + 3 * BORDER_SIZE / 2.0;
    cy = 2 * blockSize + 5 * BORDER_SIZE / 2.0;
    ;
    glColor3f(1, 1, 1);
    drawFullCircle(cx, cy, VIEW_SIZE / 13.0);
    glColor3f(0, 0, 0);
    drawEmptyCircle(cx, cy, VIEW_SIZE / 13.0);
    value = sukoBlocks[3].value + sukoBlocks[4].value + sukoBlocks[6].value + sukoBlocks[7].value;
    num[0] = value / 10 + '0';
    num[1] = value % 10 + '0';
    drawText(cx - 0.5, cy - 0.3, num, 1);

    // 5,6,8,9
    cx = 2 * blockSize + 5 * BORDER_SIZE / 2.0;
    ;
    cy = cx;
    glColor3f(1, 1, 1);
    drawFullCircle(cx, cy, VIEW_SIZE / 13.0);
    glColor3f(0, 0, 0);
    drawEmptyCircle(cx, cy, VIEW_SIZE / 13.0);
    value = sukoBlocks[4].value + sukoBlocks[5].value + sukoBlocks[7].value + sukoBlocks[8].value;
    num[0] = value / 10 + '0';
    num[1] = value % 10 + '0';
    drawText(cx - 0.5, cy - 0.3, num, 1);

    // Color Sums

    int redSum = 0, greenSum = 0, blueSum = 0;
    for (int i = 0; i < 9; i++)
    {
        if (sukoBlocks[i].group == RED)
            redSum += sukoBlocks[i].value;
        else if (sukoBlocks[i].group == GREEN)
            greenSum += sukoBlocks[i].value;
        else
            blueSum += sukoBlocks[i].value;
    }

    // RED Circle
    glColor3f(1, 0.3, 0.3);
    drawFullCircle(blockSize / 2.0 + BORDER_SIZE, VIEW_SIZE + BORDER_SIZE * 4, VIEW_SIZE / 13.0);
    num[0] = redSum / 10 + '0';
    num[1] = redSum % 10 + '0';
    drawText(blockSize / 2.0 + BORDER_SIZE - 0.5, VIEW_SIZE + BORDER_SIZE * 4 - 0.3, num, 1);

    // GREEN Circle
    glColor3f(0.3, 1, 0.3);
    drawFullCircle(3 * blockSize / 2.0 + 2 * BORDER_SIZE, VIEW_SIZE + BORDER_SIZE * 4, VIEW_SIZE / 13.0);
    num[0] = greenSum / 10 + '0';
    num[1] = greenSum % 10 + '0';
    drawText(3 * blockSize / 2.0 + 2 * BORDER_SIZE - 0.5, VIEW_SIZE + BORDER_SIZE * 4 - 0.3, num, 1);

    // BLUE Circle
    glColor3f(0.3, 0.3, 1);
    drawFullCircle(5 * blockSize / 2.0 + 3 * BORDER_SIZE, VIEW_SIZE + BORDER_SIZE * 4, VIEW_SIZE / 13.0);
    num[0] = blueSum / 10 + '0';
    num[1] = blueSum % 10 + '0';
    drawText(5 * blockSize / 2.0 + 3 * BORDER_SIZE - 0.5, VIEW_SIZE + BORDER_SIZE * 4 - 0.3, num, 1);

    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && boolReveal == 0)
        boolReveal = 1;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && boolReveal == 1)
        boolReveal = 0;
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
        generateBoard();
}

int main(int argc, char **argv)
{

    // init GLUT and create Window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Suko Generator");

    glClearColor(0.25, 0.25, 0.25, 0.0);                    // black background
    glMatrixMode(GL_PROJECTION);                            // setup viewing projection
    glLoadIdentity();                                       // start with identity matrix
    glOrtho(0.0, VIEW_SIZE, 0.0, VIEW_SIZE + 2, -1.0, 1.0); // setup a viewing world

    generateBoard();

    // register callbacks
    glutIdleFunc(renderScene);
    glutDisplayFunc(renderScene);
    glutMouseFunc(mouse);

    // enter GLUT event processing cycle
    glutMainLoop();

    return 1;
}