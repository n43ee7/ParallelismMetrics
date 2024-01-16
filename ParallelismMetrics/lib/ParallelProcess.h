
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <sys/wait.h>
#include <sys/types.h>

#define SYSTEM_PERM_READ 1024 // System permissible Read limit
#define HOZ_STR_VAL 25  // Data String horizontal limit 

typedef struct Data{
    int Num;
    char key[HOZ_STR_VAL];
} Data;

void RecordPrinter(Data *block, int i, int control);
void ParallelProcessor(int ProcessLimit, Data *block, int BlockLength); 
void StructBubbleSort(Data *block, int n);
void CompoundMemoryAcc(Data *block, int BlockLength, int prFA[2]);



// Utility function to print data 
void RecordPrinter(Data *block, int i, int control){
    // Print Struct data Data validation
    int a; 
    for (a = 0; a < i; a++) {
        printf("%s | %d \n", (block+a)->key, (block+a)->Num);
    }

    // Control Flag to return total count aswell
    if (control == 1) {
        printf("[PROCESS CONTROL] BLOCK VALUE: %d", a);
    }
    
}

// Single Process based Bubble Sort 
void StructBubbleSort(Data *block, int n){
    
    int i, j;
    for (i = 0; i < n-1; i++)  {
        for (j = 0; j < n-1-i; j++) {
            if (block[j].Num > block[j+1].Num) {
               Data temp = block[j+1];
               block[j+1] = block[j];
               block[j] = temp;
            }
        }
    }
}

// Utility function to merge multi-process blocks of Data**
void StructMergeTool(int opr, Data *block1, Data *block2, int b1_s, int b2_s, Data *output){
    // REFACTOR REQUIRED
    if (opr == 1) {
        // Use Block1 and Size 1 ONLY rest NULL
        Data temp[b1_s];

        for (int i = 0; i < b1_s; i++) {
            temp[i] = block1[i];
        }

        int i = 0, j = b1_s/2, count = i;
        while (i < b1_s/2 && j < b1_s) {
            if (temp[i].Num < temp[j].Num) {
                block1[count++] = temp[i++];
            } else {
                block1[count++] = temp[j++];
            }
        }

        if (i == b1_s/2) {
            while (j < b1_s) {
                block1[count++] = temp[j++];
            }
        } else {
            while (i < b1_s/2) {
                block1[count++] = temp[i++];
            }
        }

    } else if (opr == 2) {
        // Initialize indexing values for three arrays
        int i = 0, j = 0, k = 0;
   
        // Traverse two input arrays
        while (i < b1_s && j < b2_s) {
            if (block1[i].Num < block2[j].Num) {
                output[k++] = block1[i++];
            } else {
                output[k++] = block2[j++];
            }
        }
    
        // Append elements to output array
        while (i < b1_s) {
            output[k++] = block1[i++];
        }
        while (j < b2_s) {
            output[k++] = block2[j++];
        }
    }
}

// Creating a unified process write to memory between Jobs (Piping)
void CompoundMemoryAcc(Data *block, int BlockLength, int prFA[2]){

    close(prFA[0]); // Process FD 0 reached cancelation 
    write(prFA[1], block, BlockLength*sizeof(Data)); // Process FD 1 Write to block
    close(prFA[1]); // After write cancelation 
}
