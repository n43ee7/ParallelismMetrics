// Nabeel Nayyar | 1001842770
// CSE 3320 (Operating Systems)
// Assignment 2 Part 2
//  > Header File

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>
#include <string.h>

// Process Length 
#define MAX_RECORD_ENTRY 10000
#define HOZ_STR_VAL 25 

// Cant fit 10k elements in typedef struct? [TA HELP]
struct Data {
    int Num;
    char key[HOZ_STR_VAL];
};