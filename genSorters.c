#include "genSorters.h"


void gSwapVariables(void* variableA, void* variableB, uint64_t variableSize) {

    // Function used to swap the content of two variables of any type.
    // The main idea is to swap each byte of the variables, that is, 
    // swap each 8-bit block until all bytes of both variables have 
    // their contents swapped.

    int8_t tempContentSwapper;

    // Casting the pointers to 1 byte-pointers:
    int8_t* auxVariableA = (int8_t *)variableA;
    int8_t* auxVariableB = (int8_t *)variableB;

    // Swapping the Content:
    while ((variableSize--) > 0) {
        tempContentSwapper = *auxVariableA;
        *auxVariableA = *auxVariableB;
        *auxVariableB = tempContentSwapper;

        // Advancing to the next memory address:
        auxVariableA += 1;
        auxVariableB += 1;
    }

    return;
}

/*
void gSwapVariables(void* variableA, void* variableB, uint64_t variableSize) {
    void* auxVariable = malloc(variableSize);
    memcpy(auxVariable, variableA, variableSize);
    memcpy(variableA, variableB, variableSize);
    memcpy(variableB, auxVariable, variableSize);
    free(auxVariable);

    return;
}
*/

void gMergeArrays(compareFunctionGenSort compareF, void* startPointer, void* endPointer, uint64_t dataSize) {
    if (!startPointer || !endPointer || !compareF) return;

    uint64_t arraySize = ((endPointer - startPointer) / dataSize) + 1;

    void* auxMergeArray = malloc(arraySize * dataSize);
    void* auxMergeArrayTempPointer = auxMergeArray;
    if (auxMergeArray == NULL) {
        fprintf(stderr, "Error: Cannot allocate memory for the auxiliar array in generic Merge Sort.\n");
        return;
    }

    int comparisonResult;

    void* middlePointer = startPointer + (arraySize / 2) * dataSize;

    void *auxPointer1 = startPointer, *auxPointer2 = middlePointer;
    while ((auxPointer1 < middlePointer) && (auxPointer2 <= endPointer)) {
        comparisonResult = compareF(auxPointer1, auxPointer2);

        if (comparisonResult < 0) {
            gSwapVariables(auxMergeArrayTempPointer, auxPointer1, dataSize);
            auxMergeArrayTempPointer += dataSize;
            auxPointer1 += dataSize;
            continue;
        }

        if (comparisonResult > 0) {
            gSwapVariables(auxMergeArrayTempPointer, auxPointer2, dataSize);
            auxMergeArrayTempPointer += dataSize;
            auxPointer2 += dataSize;
            continue;
        }

        // Comparison result == 0:
        gSwapVariables(auxMergeArrayTempPointer, auxPointer1, dataSize);
        auxMergeArrayTempPointer += dataSize;
        gSwapVariables(auxMergeArrayTempPointer, auxPointer2, dataSize);
        auxMergeArrayTempPointer += dataSize;
        
        auxPointer1 += dataSize;
        auxPointer2 += dataSize;
    }

    while (auxPointer1 < middlePointer) {
        gSwapVariables(auxMergeArrayTempPointer, auxPointer1, dataSize);
        auxMergeArrayTempPointer += dataSize;
        auxPointer1 += dataSize;
    }

    while (auxPointer2 <= endPointer) {
        gSwapVariables(auxMergeArrayTempPointer, auxPointer2, dataSize);
        auxMergeArrayTempPointer += dataSize;
        auxPointer2 += dataSize;
    }

    auxMergeArrayTempPointer = auxMergeArray;
    uint64_t auxCounter = 0;
    while (auxCounter < arraySize) {
        gSwapVariables(auxMergeArrayTempPointer, startPointer, dataSize);
        auxMergeArrayTempPointer += dataSize;
        startPointer += dataSize;
        auxCounter++;
    }

    free(auxMergeArray);

    return;
}


void gRecursiveMergeSort(compareFunctionGenSort compareF, void* arrayPointer, uint64_t arraySize, uint64_t dataSize) {
    if (!arrayPointer || arraySize < 2) return;

    if (arraySize == 2) {
        if (compareF(arrayPointer, arrayPointer + dataSize) > 0) gSwapVariables(arrayPointer, arrayPointer + dataSize, dataSize);
        return;
    }

    uint64_t halfArraySize = arraySize / 2;

    gRecursiveMergeSort(compareF, arrayPointer, halfArraySize, dataSize);
    gRecursiveMergeSort(compareF, arrayPointer + dataSize * halfArraySize, arraySize - halfArraySize, dataSize);

    gMergeArrays(compareF, arrayPointer, arrayPointer + dataSize * (arraySize - 1), dataSize);

    return;
}


void gRecursiveQuickSort(compareFunctionGenSort compareF, void* arrayPointer, uint64_t arraySize, uint64_t dataSize) {
    //if (!arrayPointer) return;

    if (arraySize < 2) return;
    if (arraySize == 2) {
        if (compareF(arrayPointer, arrayPointer + dataSize) > 0) gSwapVariables(arrayPointer, arrayPointer + dataSize, dataSize);
        return;
    }

    /* Strategically choosing the pivot based on the median between the first, last and central element of the array.
     This technique, known as "median of three", helps reduce the likelihood of unbalanced partitions, which can lead 
     to poor algorithm performance — O(n²) case. By using the median of these three elements, we increase the chance of 
     obtaining a pivot closer to the central value, promoting more balanced partitions and substantially improving the 
     overall efficiency of Quicksort.
    */
    
    /*
    uint64_t medianIndex = arraySize / 2;

    void* startPointer = arrayPointer;
    void* medianPointer = arrayPointer + dataSize * medianIndex;
    void* endPointer = arrayPointer + dataSize * (arraySize - 1);
    
    if (compareF(startPointer, medianPointer) >= 0) gSwapVariables(startPointer, medianPointer, dataSize);
    if (compareF(medianPointer, endPointer) >= 0) gSwapVariables(medianPointer, endPointer, dataSize);
    if (compareF(startPointer, medianPointer) >= 0) gSwapVariables(startPointer, medianPointer, dataSize);

    if (arraySize == 3) return;

    // Ensuring that the best pivot of the median between the first, 
    // last and middle elements of the array is at the end of the array:
    gSwapVariables(medianPointer, endPointer, dataSize);
    */
    
    void* pivotPointer = arrayPointer + dataSize * (arraySize - 1);

    void* auxIndexI = arrayPointer;
    void* auxIndexJ = arrayPointer + (arraySize - 2) * dataSize;

    while (compareF(auxIndexI, pivotPointer) <= 0 && auxIndexI < auxIndexJ) auxIndexI += dataSize;
    while (compareF(auxIndexJ, pivotPointer) > 0 && auxIndexI < auxIndexJ) auxIndexJ -= dataSize;

    while (auxIndexI < auxIndexJ) {
        gSwapVariables(auxIndexI, auxIndexJ, dataSize);
        while (compareF(auxIndexI, pivotPointer) <= 0) auxIndexI += dataSize;
        while (compareF(auxIndexJ, pivotPointer) > 0) auxIndexJ -= dataSize;
    }

    if (compareF(auxIndexI, pivotPointer) >= 0) gSwapVariables(auxIndexI, pivotPointer, dataSize);

    uint64_t auxIndexIFinalIndex = (uint64_t)(auxIndexI - arrayPointer) / dataSize;

    gRecursiveQuickSort(compareF, arrayPointer, auxIndexIFinalIndex, dataSize);
    gRecursiveQuickSort(compareF, auxIndexI + dataSize, arraySize - (auxIndexIFinalIndex + 1), dataSize);

    return;  
}


void gHeapfy(compareFunctionGenSort compareF, void* arrayPointer, uint64_t arraySize, uint64_t dataSize, uint64_t index, bool doMaxHeap) {
    if (!arrayPointer || arraySize < 2) return;

    if (!compareF || dataSize == 0) {
        if (!compareF) fprintf(stderr, "Error: Cannot apply generic Heapfy if the compare function is NULL.\n");
        if (dataSize == 0) fprintf(stderr, "Error: DataSize parameter for generic Heapfy function cannot be zero.\n");
        return;
    }

    // doMaxHeap parameter: if true => Make a MaxHeap; else Make a MinHeap:

    void* elementPointer = arrayPointer + dataSize * index;

    void* swapSonPointer = elementPointer;
    
    uint64_t leftSon = 2 * index + 1;
    uint64_t rightSon = 2 * index + 2;

    void* leftSonPointer = arrayPointer + dataSize * leftSon;
    void* rightSonPointer = arrayPointer + dataSize * rightSon;

    if (doMaxHeap) {
        if (leftSon < arraySize && compareF(leftSonPointer, swapSonPointer) > 0) swapSonPointer = leftSonPointer;
        if (rightSon < arraySize && compareF(rightSonPointer, swapSonPointer) > 0) swapSonPointer = rightSonPointer;
    } else {
        // MinHeap:
        if (leftSon < arraySize && compareF(leftSonPointer, swapSonPointer) < 0) swapSonPointer = leftSonPointer;
        if (rightSon < arraySize && compareF(rightSonPointer, swapSonPointer) < 0) swapSonPointer = rightSonPointer;
    }

    if (swapSonPointer == elementPointer) return;

    gSwapVariables(elementPointer, swapSonPointer, dataSize);

    if (swapSonPointer == leftSonPointer) { 
        gHeapfy(compareF, arrayPointer, arraySize, dataSize, leftSon, doMaxHeap); 
    } else gHeapfy(compareF, arrayPointer, arraySize, dataSize, rightSon, doMaxHeap);
    
    return;
}


void gBubbleSort(compareFunctionGenSort compareF, void* arrayPointer, uint64_t arraySize, uint64_t dataSize) {
    if (!arrayPointer || arraySize < 2) return;

    if (!compareF || dataSize == 0) {
        if (!compareF) fprintf(stderr, "Error: Cannot apply generic Bubble Sort if the Array if the compare function is NULL.\n");
        if (dataSize == 0) fprintf(stderr, "Error: DataSize parameter for generic Bubble Sort function cannot be zero.\n");
        return;
    }

    // Trivial Case:
    if (arraySize == 2) {
        if (compareF(arrayPointer, arrayPointer + dataSize) > 0) gSwapVariables(arrayPointer, arrayPointer + dataSize, dataSize);
        return;
    }

    bool changeMade = false;
    int comparisonResult;

    arraySize--;
    void* endPointer = arrayPointer + dataSize * arraySize;

    for (uint64_t auxCounter1 = 0; auxCounter1 < arraySize; auxCounter1++) {
        // Variable to determine whether there was a swap between values.
        // If there are no changes in a loop iteration, then the
        // array is already sorted and we can terminate the
        // function prematurely:
        changeMade = false;
        void* endPointer = arrayPointer + (arraySize - auxCounter1) * dataSize;

        for (void* auxPointer = arrayPointer; auxPointer < endPointer; auxPointer += dataSize) {
            comparisonResult = compareF(auxPointer, auxPointer + dataSize);
            
            if (comparisonResult > 0) {
                gSwapVariables(auxPointer, auxPointer + dataSize, dataSize);
                changeMade = true;
            }
        }

        if (changeMade == false) return;
    }

    return;
}


void gMergeSort(compareFunctionGenSort compareF, void* arrayPointer, uint64_t arraySize, uint64_t dataSize) {
    if (!arrayPointer || arraySize < 2) return;

    if (!compareF || dataSize == 0) {
        if (!compareF) fprintf(stderr, "Error: Cannot apply generic Merge Sort if the Array if the compare function is NULL.\n");
        if (dataSize == 0) fprintf(stderr, "Error: DataSize parameter for generic Merge Sort function cannot be zero.\n");
        return;
    }

    gRecursiveMergeSort(compareF, arrayPointer, arraySize, dataSize);

    return;
}


void gQuickSort(compareFunctionGenSort compareF, void* arrayPointer, uint64_t arraySize, uint64_t dataSize) {
    if (!arrayPointer || arraySize < 2) return;

    if (!compareF || dataSize == 0) {
        if (!compareF) fprintf(stderr, "Error: Cannot apply generic Quick Sort if the Array if the compare function is NULL.\n");
        if (dataSize == 0) fprintf(stderr, "Error: DataSize parameter for generic Quick Sort function cannot be zero.\n");
        return;
    }

    gRecursiveQuickSort(compareF, arrayPointer, arraySize, dataSize);

    return;
}

void gHeapSort(compareFunctionGenSort compareF, void* arrayPointer, uint64_t arraySize, uint64_t dataSize) {
    if (!arrayPointer || arraySize < 2) return;

    if (!compareF || dataSize == 0) {
        if (!compareF) fprintf(stderr, "Error: Cannot apply generic Bubble Sort the Array if the compare function is NULL.\n");
        if (dataSize == 0) fprintf(stderr, "Error: DataSize parameter for generic Bubble Sort function cannot be zero.\n");
        return;
    }

    for (uint64_t i = arraySize - 1; i > 0; i--) gHeapfy(compareF, arrayPointer, arraySize, dataSize, i, true);
    gHeapfy(compareF, arrayPointer, arraySize, dataSize, 0, true);

    for (uint64_t i = arraySize - 1; i > 0; i--) {
        gSwapVariables(arrayPointer, arrayPointer + dataSize * i, dataSize);
        gHeapfy(compareF, arrayPointer, i, dataSize, 0, true);
    }
}

// ---------------------------------------------------------------------------- //

uint64_t getuint64tMin(uint64_t a, uint64_t b) { return (a < b) ? a : b; }


bool checkArrayIsOrdered(compareFunctionGenSort compareF, void* arrayPointer, uint64_t arraySize, uint64_t dataSize) {
    if (!arrayPointer || arraySize < 2) return true;

    void* lastPointer = arrayPointer + dataSize * (arraySize - 1);
    while (arrayPointer < lastPointer) {
        if (compareF(arrayPointer, arrayPointer + dataSize) > 0) return false;
        arrayPointer += dataSize;
    }

    return true;
}


uint64_t generateRandomIndex(uint64_t start, uint64_t end) {
    if (end < start) return 0;

    //srand((unsigned int)time(NULL));

    uint64_t intervalRange = end - start;

    if (intervalRange > RAND_MAX) {
        uint64_t offsetValue = (uint64_t)ceil((long double)intervalRange / (long double)RAND_MAX);

        // Generating previous values to clean
        // the random number generator:
        ((uint64_t)rand() * offsetValue);
        ((uint64_t)rand() % offsetValue);

        return getuint64tMin(start + ((uint64_t)rand() * offsetValue) + ((uint64_t)rand() % offsetValue), start + intervalRange);
    } else {
        // The time squared and division by 13 is just to improve the randomness:
        return ((uint64_t)rand() + (uint64_t)(time(NULL) * time(NULL)) / 13) % (intervalRange + 1) + start;
    }    
}


void gRecursiveStoogeSort(compareFunctionGenSort compareF, void* arrayPointer, uint64_t arraySize, uint64_t dataSize) {
    if (arraySize == 3) {
        if (compareF(arrayPointer, arrayPointer + dataSize) > 0) gSwapVariables(arrayPointer, arrayPointer + dataSize, dataSize);
        if (compareF(arrayPointer + dataSize, arrayPointer + 2 * dataSize) > 0) gSwapVariables(arrayPointer + dataSize, arrayPointer + 2 * dataSize, dataSize);
        if (compareF(arrayPointer, arrayPointer + dataSize) > 0) gSwapVariables(arrayPointer, arrayPointer + dataSize, dataSize);
        return;
    }

    uint64_t subArraySize = (uint64_t)ceil((double_t)arraySize * 2 / 3);

    gRecursiveStoogeSort(compareF, arrayPointer, subArraySize, dataSize);
    gRecursiveStoogeSort(compareF, arrayPointer + dataSize * (arraySize - subArraySize), subArraySize, dataSize);
    gRecursiveStoogeSort(compareF, arrayPointer, subArraySize, dataSize);

    return;
}


void gBogoSort(compareFunctionGenSort compareF, void* arrayPointer, uint64_t arraySize, uint64_t dataSize) {
    if (!arrayPointer || arraySize < 2) return;

    if (!compareF || dataSize == 0) {
        if (!compareF) fprintf(stderr, "Error: Cannot apply generic Bogo Sort if the compare function is NULL.\n");
        if (dataSize == 0) fprintf(stderr, "Error: DataSize parameter for generic Stooge Sort function cannot be zero.\n");
        return;
    }

    // Randomizing the array elements:
    do {
        for (uint64_t i = arraySize - 1; i > 0; i--) {
            gSwapVariables(arrayPointer + dataSize * i, arrayPointer + dataSize * generateRandomIndex(0, i), dataSize);
        }
    } while (!checkArrayIsOrdered(compareF, arrayPointer, arraySize, dataSize));

    return;
}

void gGoodEnough(compareFunctionGenSort compareF, void* arrayPointer, uint64_t arraySize, uint64_t dataSize) {
    // hmm, eh, it's good enough.
    return;
}

void gStoogeSort(compareFunctionGenSort compareF, void* arrayPointer, uint64_t arraySize, uint64_t dataSize) {
    if (!arrayPointer || arraySize < 2) return;

    if (!compareF || dataSize == 0) {
        if (!compareF) fprintf(stderr, "Error: Cannot apply generic Stooge Sort the Array if the compare function is NULL.\n");
        if (dataSize == 0) fprintf(stderr, "Error: DataSize parameter for generic Stooge Sort function cannot be zero.\n");
        return;
    }

    if (arraySize == 2) {
        if (compareF(arrayPointer, arrayPointer + dataSize) > 0) gSwapVariables(arrayPointer, arrayPointer + dataSize, dataSize);
        return;
    }

    gRecursiveStoogeSort(compareF, arrayPointer, arraySize, dataSize);

    return;
}