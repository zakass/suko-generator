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