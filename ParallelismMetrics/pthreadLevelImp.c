// 			(c) Nabeel Nayyar 
// ---------------------------------------------------------------
// Parallel Processing Metrics using the most inefficient Method
// BUBBLE SORT....
// ---------------------------------------------------------------
// Thread Level (pthread) Implementation for singular and multiple 
// Processes
//
//
//  NOTES:
//      > Preallocated array values for functions reduces process time
//      > Using ThreadProcess.h slows down runtime by 0.004~ sec
//      > ONLY RUN WITH ./a.out -d ELSE PROGRAM WILL NOT WORK

#include "lib/ThreadProcess.h"

// Global Variables
int DEBUGGER_STAT = 0;
int ThreadLim;

// Declaring the functions used in the instance of the program
void ThreadProcess(); 
void StructSortAlt (struct Data block[MAX_RECORD_ENTRY][MAX_RECORD_ENTRY/ThreadLim], struct Data ListedRecord[MAX_RECORD_ENTRY], int start, int end); 
void StructMergeStruct (struct Data split1[MAX_RECORD_ENTRY], struct Data split2[MAX_RECORD_ENTRY], struct Data ListedRecord[MAX_RECORD_ENTRY], int mid1, int mid2); 
void *StructBubbleSort (void *ThreadPtr); 
void RecordPrinter(struct Data block[], int i, int control);

// Declaring the Struct for entries
// NOTE: Tried to use this as a parameter but it does not work
struct Data ReadEntry[MAX_RECORD_ENTRY];

int main(int argc, char *argv[]){

    // Some Exotic modifications 
    if (strcmp(argv[1], "-d") == 0) {
        DEBUGGER_STAT = 1; // Defining that there will be debugger outputs
    } else {
        DEBUGGER_STAT = 0; // NO OUTPUTS
    }

    // Defining entry varaibles
    FILE *inStream; 
    clock_t t;
    double ProcessTime;
    char buffer[MAX_RECORD_ENTRY];
    char *delimeter;
    
    // Opening Data File as Read
    inStream = fopen("Dataset/data.csv", "r");

    // File exception handling
    if (inStream == NULL) {
        printf("[!] Error occured while opening data file.\n");
        exit(EXIT_FAILURE);
    }

    // Reading from the input file till the Struct Limit 10k
  	for (size_t i = 0; i < MAX_RECORD_ENTRY; i++) {
        fscanf(inStream, "%s", buffer);

        delimeter = strtok(buffer, ",");
        strcpy(ReadEntry[i].key, delimeter);
        delimeter = strtok(NULL, buffer);
        ReadEntry[i].Num = atoi(delimeter);
    }
    
    // Upon sucessfull Read Close file
    fclose(inStream); 

    // Debugging outputs to check struct handling
    if (DEBUGGER_STAT == 1) {
        printf("==================READ DATA=======================\n");
        RecordPrinter(ReadEntry, MAX_RECORD_ENTRY, 1);
        printf("\n");
        printf("==================END READ========================\n");
    }

    // Menu to employ processes
    // Loop breaking for now but adjust this for good rt
    // Time the system Process time (IGNORE THE SELECTION TIME? [TA HELP])
    
    while (1) {
        printf("Set the number of permissable Processes: ");
        scanf("%d", &ThreadLim); 

        // Updated Process security to not lock out device
        if (ThreadLim == 1 ) {      
            printf("NUMBER OF PROCESS THREADS: %d\n", ThreadLim);
            sleep(2);
            printf("[PROCESS STARTED]\n");
            // Start system Clock    
            t = clock();
            ThreadProcess();        
            // Metric Processing     
            t = clock() - t;
            break;
        } else if (ThreadLim == 2) {
            printf("NUMBER OF PROCESS THREADS: %d\n", ThreadLim);
            sleep(2);
            printf("[PROCESS STARTED]\n");
            // Start system Clock    
            t = clock();
            ThreadProcess();        
            // Metric Processing     
            t = clock() - t;
            break;
        } else if (ThreadLim == 4) {
            printf("NUMBER OF PROCESS THREADS: %d\n", ThreadLim);
            sleep(2);
            printf("[PROCESS STARTED]\n");
            // Start system Clock    
            t = clock();
            ThreadProcess();        
            // Metric Processing     
            t = clock() - t;            break;
        } else if (ThreadLim == 10) {
            printf("NUMBER OF PROCESS THREADS: %d\n", ThreadLim);
            sleep(2);
            printf("[PROCESS STARTED]\n");
            
            // Start system Clock    
            t = clock();
            ThreadProcess();        
            // Metric Processing     
            t = clock() - t;
            break;
        } else {
            printf("Invalid permissable Processes requested\n Choose from [1], [2], [4], [10]\n");
        }
    }
    printf("[PROCESS FINISHED FOR (%d) THREADS]\n", ThreadLim);
    sleep(5);    
    // Debugging outputs to check struct handling
    if (DEBUGGER_STAT == 1) {
        printf("==================READ DATA=======================\n");
        RecordPrinter(ReadEntry, MAX_RECORD_ENTRY, 1);
        printf("\n");
        printf("==================END READ========================\n");
    }
    
    printf("[!] System waiting 5(sec), awaiting metrics...\n");
    sleep(5);
    ProcessTime = ((double)t)/CLOCKS_PER_SEC;
    printf("PROCESS time for %d THREADS: %f (s)\n", ThreadLim, ProcessTime);
    return 0;
}

// Function for Starting a threaded process for N numbers
void ThreadProcess(){
    // Entry Variables for Threading the process
    int ProcessLimit = ThreadLim;
    
    // Declaring the threaded object array of threads for Process Limit provided
    pthread_t thread_obj[ProcessLimit];

    // Deploying Threads on a bubble sort
    for (long thread_id = 0; thread_id < ProcessLimit; thread_id++) {
        int threadlam = pthread_create(&thread_obj[thread_id], NULL, StructBubbleSort, (void *)thread_id);
        
        // Exception handling for failed thread creation
        if (threadlam == 1) {
            printf("[!] Error encountered while creating a thread process...\n");
            exit(EXIT_FAILURE); // Exiting Thread failure
        }
    }

    // Joining threaded process for child processes
    for (size_t i = 0; i < ProcessLimit; i++) {
        pthread_join(thread_obj[i], NULL);
    }

    // Separating data to thread processes 
    struct Data buffer[ProcessLimit][MAX_RECORD_ENTRY/ProcessLimit];
    int a = 0; // Counting processes split
    for (size_t i = 0; i < ProcessLimit; i++) {
        for (size_t j = 0; j < (MAX_RECORD_ENTRY/ProcessLimit); j++) {
            buffer[i][j] = ReadEntry[a++];
        }
    }
    //printf("A:: %d",a)
    StructSortAlt(buffer, ReadEntry, 0, ProcessLimit-1);
}

// Sorting Package for Merging Struct Values split for each Thread Opeation
void StructSortAlt (struct Data block[MAX_RECORD_ENTRY][MAX_RECORD_ENTRY/ThreadLim], struct Data ListedRecord[], int start, int end) {
    
    // Merge sort, sort of
    int length = MAX_RECORD_ENTRY/ThreadLim;
    
    // Exception handling for end level entries
    if (start == end) {     // Block end
        for (size_t a = 0; a < length; a++) {
            // ReadEntry = (struct Data *)malloc(a * sizeof(struct Data));
            // ReadEntry = block;
            ListedRecord[a] = block[start][a];
        }
        return;
    } else if (end - start == 1) {
        // HERE
        //int i  = 0;
        //int i1 = 0; 
        //int i2 = 0; 
        //while (i < length && i1 < length) {
        //    if (block[i][0].Num < block[i1][0].Num) {
        //        ListedRecord[i2] = block[i][0];
        //        i++;i2++;
        //    } else {
        //        ListedRecord[i2] = block[i1][0];
        //        i1++;i2++; // This works aparently
        //    }
        //}
        //while (i < length && i1 < length) {
        //    ListedRecord[i2] = block[i][0];
        //    ListedRecord[i2] = block[i1][0];
        //    i++; i1++; i2++;
        //}
        // HERE
        StructMergeStruct(block[start], block[end], ListedRecord, length, length);
        return;
    }
     
    // Rescurive divide into subarrays by Creating recursion calls with the chopped data 
    // from (start->end /2) for next process
    // Dont touch them or they will crash 
    struct Data split1[length * (((start + end) / 2) - start+1)];
    StructSortAlt(block, split1, start, (start + end) / 2);
    struct Data split2[length * (end - ((start + end) / 2))];
    StructSortAlt(block, split2, (start + end) / 2 + 1, end);

    // Find middle values for the merge of data from sorted entries 
    int mid1 = length * (((start + end) / 2) - start + 1);
    int mid2 = length * (end - ((start + end) / 2));
    
    // Merge the first array and the second array as output array using mid1 and mid 2 values
    StructMergeStruct(split1, split2, ListedRecord, mid1, mid2);
}

void StructMergeStruct (struct Data split1[MAX_RECORD_ENTRY], struct Data split2[MAX_RECORD_ENTRY], struct Data ListedRecord[MAX_RECORD_ENTRY], int mid1, int mid2) {
    
    // Run thru record and perform swapping (DONT SORT)
    int i  = 0;
    int i1 = 0; 
    int i2 = 0;
   
    while (i < mid1 && i1 < mid2) {
        if (split1[i].Num < split2[i1].Num) {
            ListedRecord[i2] = split1[i];
            i++;i2++;
        } else {
            ListedRecord[i2] = split2[i1];
            i1++;i2++; // This works aparently
        }
    }
    // Now merge
    while (i < mid1 && i1 < mid2) {
        ListedRecord[i2] = split1[i];
        ListedRecord[i2] = split2[i1];
        i++; i1++; i2++;
    }
}

// Structure Bubble Sort with complement to Thread ID for the system
void *StructBubbleSort (void *ThreadPtr) {

    long Thread_ID = (long)ThreadPtr;
    int loc = Thread_ID*(MAX_RECORD_ENTRY/ThreadLim);
    int lloc1 = Thread_ID*(MAX_RECORD_ENTRY/ThreadLim) + MAX_RECORD_ENTRY/ThreadLim;

    for (size_t i = loc; i < lloc1 - 1; i++) {
        for (size_t j = loc; j < lloc1 - i -1 + loc; j++) {
            if (ReadEntry[j].Num > ReadEntry[j + 1].Num) {
                struct Data buffer = ReadEntry[j];
                ReadEntry[j] = ReadEntry[j + 1];
                ReadEntry[j + 1] = buffer;
            }
        }
    }    

    // Terminate the calling thread
    pthread_exit(NULL);
}

// Utility function to print data [R]
void RecordPrinter(struct Data block[], int i, int control){
    // Print Struct data Data validation
    int a; 
    for (a = 0; a < i; a++) {
        printf("%s | %d \n", block[a].key, block[a].Num);
    }

    // Control Flag to return total count aswell
    if (control == 1) {
        printf("[PROCESS CONTROL] BLOCK VALUE: %d", a);
    }
}
