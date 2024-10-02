#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_PROCESS 10
#define MAX_RESOURCE 10

// Function to calculate the need matrix
void findNeed(int need[MAX_PROCESS][MAX_RESOURCE], int max[MAX_PROCESS][MAX_RESOURCE], int alloc[MAX_PROCESS][MAX_RESOURCE], int processNo, int resourceNo) {
    for (int i = 0; i < processNo; i++) {
        for (int j = 0; j < resourceNo; j++) {
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }
}

// Function to handle additional resources
void handleAdditionalResource(int max[MAX_PROCESS][MAX_RESOURCE], int extra[MAX_PROCESS][MAX_RESOURCE], int processNo, int resourceNo) {
    for (int i = 0; i < processNo; i++) {
        for (int j = 0; j < resourceNo; j++) {
            max[i][j] += extra[i][j];
        }
    }
}

// Function to check if a process can be executed
bool isExecutable(int need[], int aval[], int resourceNo) {
    for (int i = 0; i < resourceNo; i++) {
        if (need[i] > aval[i]) {
            return false;
        }
    }
    return true;
}

// Function to free resources after execution
void freeResources(int max[], int aval[], int resourceNo) {
    for (int i = 0; i < resourceNo; i++) {
        aval[i] += max[i];
    }
}

// Banker's Algorithm
void bankersAlgo(int alloc[MAX_PROCESS][MAX_RESOURCE], int max[MAX_PROCESS][MAX_RESOURCE], int aval[], int extra[MAX_PROCESS][MAX_RESOURCE], int processNo, int resourceNo) {
    int need[MAX_PROCESS][MAX_RESOURCE];
    bool executed[MAX_PROCESS] = {false};  // Array to keep track of executed processes
    int sequence[MAX_PROCESS];  // To store the safe sequence
    int seqIndex = 0;

    // Add extra resources to max and calculate the need matrix
    handleAdditionalResource(max, extra, processNo, resourceNo);
    findNeed(need, max, alloc, processNo, resourceNo);

    while (true) {
        bool none_executed = true;

        for (int i = 0; i < processNo; i++) {
            if (!executed[i] && isExecutable(need[i], aval, resourceNo)) {
                freeResources(max[i], aval, resourceNo);
                executed[i] = true;
                sequence[seqIndex++] = i;
                none_executed = false;
            }
        }

        bool allExecuted = true;
        for (int i = 0; i < processNo; i++) {
            if (!executed[i]) {
                allExecuted = false;
                break;
            }
        }

        if (allExecuted) {
            printf("Safe sequence: ");
            for (int i = 0; i < processNo; i++) {
                printf("%d ", sequence[i]);
            }
            printf("\n");
            break;
        }

        if (none_executed) {
            printf("Unsafe Sequence\n");
            break;
        }
    }
}

// Helper function to input resources and matrices
void inputResources(int alloc[MAX_PROCESS][MAX_RESOURCE], int max[MAX_PROCESS][MAX_RESOURCE], int extra[MAX_PROCESS][MAX_RESOURCE], int aval[], int *processNo, int *resourceNo) {
    printf("Enter the number of processes: ");
    scanf("%d", processNo);
    printf("Enter the number of resources: ");
    scanf("%d", resourceNo);

    for (int i = 0; i < *processNo; i++) {
        printf("Enter allocated resource for process %d [space separated]: ", i);
        for (int j = 0; j < *resourceNo; j++) {
            scanf("%d", &alloc[i][j]);
        }
    }

    for (int i = 0; i < *processNo; i++) {
        printf("Enter max resources for process %d [space separated]: ", i);
        for (int j = 0; j < *resourceNo; j++) {
            scanf("%d", &max[i][j]);
        }
    }

    for (int i = 0; i < *processNo; i++) {
        printf("Enter extra resources for process %d [space separated]: ", i);
        for (int j = 0; j < *resourceNo; j++) {
            scanf("%d", &extra[i][j]);
        }
    }

    printf("Enter available resources [space separated]: ");
    for (int i = 0; i < *resourceNo; i++) {
        scanf("%d", &aval[i]);
    }
}

int main() {
    int alloc[MAX_PROCESS][MAX_RESOURCE], max[MAX_PROCESS][MAX_RESOURCE], extra[MAX_PROCESS][MAX_RESOURCE], aval[MAX_RESOURCE];
    int processNo, resourceNo;

    // Input resource matrices and available resources
    inputResources(alloc, max, extra, aval, &processNo, &resourceNo);

    // Run the Banker's Algorithm
    bankersAlgo(alloc, max, aval, extra, processNo, resourceNo);

    return 0;
}
