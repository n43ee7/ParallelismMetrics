// 			(c) Nabeel Nayyar 
// ---------------------------------------------------------------
// Parallel Processing Metrics using the most inefficient Method
// BUBBLE SORT....
// ---------------------------------------------------------------
// System Level Threads Implementation for singular and multiple 
// Processes
//

#include "lib/ParallelProcess.h"

// Processing Functions
void N1Processes(Data *block, int BlockLenght);
void N2Processes(Data *block, int BlockLength);
void N4Processes(Data *block, int BlockLenght);
void N10Processes(Data *block, int BlockLenght);

// TODO: Set Debug mode thru argc
int main(){
    // Defining entry varaibles
    FILE *inStream; 

    // Opening Data File as Read
    inStream = fopen("Dataset/data.csv", "r");

    // File exception handling
    if (inStream == NULL) {
        printf("[!] Error occured while opening data file.\n");
        exit(0);
    }


    char Sr[SYSTEM_PERM_READ]; // Instantaneous Read LINE
    
    // Dynamically Allocating space for data
    // TODO: FIT FOR ANY FILE LENGHT 
    Data *ReadEntry = (Data *)malloc(10000 * sizeof(Data));

    // Reading from the file and adding entries to variables
    int r, i;
    do {
        r = fscanf(inStream, "%30[^,], %d\n", (ReadEntry+i)->key, &(ReadEntry+i)->Num);
        
        // Error handling
        if (r = 2) {
            i++;    // Record Read sucessfully -> Onto next
        } else if (r != 2 && !feof(inStream)) {
            printf("CSV ERR:: [!] An error encountered reading the CSV file");
            exit(1);
        }       
        if (ferror(inStream)){
            printf("EOFERR:: [!] An error encountered reading the CSV file");
            exit(1);
        }

    } while (!feof(inStream));
    
    // Upon sucessfull Read Close file
    fclose(inStream); 

    // Menu to employ processes
    int PLim;

    // Loop breaking for now but adjust this for good rt
    while (1) {
        printf("Set the number of permissable Processes: ");
        scanf("%d", &PLim); 

        // Updated Process security
        if (PLim == 1 ) {
            ParallelProcessor(PLim, ReadEntry, 10000);
            break;
        } else if (PLim == 2) {
            ParallelProcessor(PLim, ReadEntry, 10000);
            break;
        } else if (PLim == 4) {
            ParallelProcessor(PLim, ReadEntry, 10000);
            break;
        } else if (PLim == 10) {
            ParallelProcessor(PLim, ReadEntry, 10000);
            break;
        } else {
            printf("Invalid permissable Processes requested\n Choose from [1], [2], [4], [10]\n");
        }        
    }

    return 0;
}

// Parallel operation code driver
void ParallelProcessor(int ProcessLimit, Data *block, int BlockLength){

    clock_t t;
    double ProcessTime;

    printf("NUMBER OF PROCESSES: %d\n", ProcessLimit);
    sleep(2);
    printf("[PROCESS STARTED]\n");

    // Start system Clock    
    t = clock();
    
    // There will be slight latencty during selection but 0.0004s as tested.
    if (ProcessLimit == 1) {
        N1Processes(block, BlockLength);
    } else if (ProcessLimit == 2){
        N2Processes(block, BlockLength);
    } else if (ProcessLimit == 4) {
        N4Processes(block, BlockLength);
    } else if (ProcessLimit == 10) {
        N10Processes(block, BlockLength);
    }

    // Metric Processing     
    t = clock() - t;
    printf("[PROCESS FINISHED]\n");
    ProcessTime = ((double)t)/CLOCKS_PER_SEC;
    printf("PROCESS time: %f (s)\n", ProcessTime);
}

// Working with acceptable RT
void N1Processes(Data *block, int BlockLenght) {
    StructBubbleSort(block, 10000);

    // Implemented for system fairness. Each function has a static
    // latency thus one process must also face the same latency regardless
    // of the code complexity
}

// Debugging
void N2Processes(Data *block, int BlockLength) {
    int prFAD[2];
    if (pipe(prFAD) == -1) {
        printf("[!] ERR::System could not pipe. Check permissions?\n");
        exit(EXIT_FAILURE);
    }
    // Checking fork for child processes
    pid_t pid = fork();
    if (pid < 0) {
        printf("[!] ERR::System could not fork processes, exitting...\n");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Since N = 2, Sort N/2 + N/2 = N
        StructBubbleSort(block, BlockLength/2);    
        CompoundMemoryAcc(block, BlockLength/2, prFAD);

    } else if (pid > 0) {
        StructBubbleSort(&block[BlockLength/2], BlockLength/2);

        CompoundMemoryAcc(block, BlockLength, prFAD);
    } 

    // Merge all sorted arrays from children processes
    StructMergeTool(1, block, block, BlockLength, BlockLength, NULL);
}

// Unreleased
void N4Processes(Data *block, int BlockLength) {

}

// Unreleased
void N10Processes(Data *block, int BlocLenght) {

}
