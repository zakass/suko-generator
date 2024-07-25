#pragma once

#define MIN_NUM_SECTIONS 3
#define MAX_NUM_SECTIONS 4
#define NUM_BOXES 9

static const int nums[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

typedef enum group
{
    WHITE = 0,
    RED = 1,
    GREEN = 2,
    BLUE = 3
} group_t;

typedef struct block
{
    int value;
    int position; // 3 x 3, so every 3 is a row down
    group_t group;
} block_t;

static int *
randomPermutation();
static void arraySwap(int *a, int *b);
static group_t *generateGroups();
void generateBoard();

block_t sukoBlocks[NUM_BOXES];