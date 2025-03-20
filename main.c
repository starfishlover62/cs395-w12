#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>


#define FLAG_SELECTION 1
#define FLAG_QUICK 2
#define FLAG_INSERTION 4


struct int_array {
    int* array;
    unsigned size;
};

struct trial {
    unsigned num_elements;
    double selection_sort_time;
    double insertion_sort_time;
    double quick_sort_time;
};

struct data {
    struct int_array selection_sort_data;
    struct int_array insertion_sort_data;
    struct int_array quick_sort_data;
};

struct slice {
    int l;
    int r;
};

int generateArrays(unsigned, struct data*);
int runTrial(unsigned, struct trial*);
void initializeData(struct data*);
int parseCL(int, char**, struct int_array*);
int checkArray(struct int_array);

int executeSort(void (*f)(struct int_array), struct int_array arr, double* time);

void saveTime(struct timeval, struct timeval, double*);


// Sorting functions
//      Selection Sort
void printSelectionSort(struct int_array);
void selectionSort(struct int_array);
//      Insertion Sort
void printInsertionSort(struct int_array, unsigned);
void InsertionSort(struct int_array);
//      Quick Sort
void printQuickSort(struct int_array, struct slice);
void QuickSortCaller(struct int_array);
void QuickSort(struct int_array, struct slice);
int hoarePartition(struct int_array, struct slice);
void swap(int*, int*);


// Output functions
//      Table Output
void printTable(struct trial*,unsigned);
void printTableHead();
void printTableRow(struct trial);
void printTableFoot();

//      Sorting Output

int main() {

    srand(time(NULL));
    unsigned num_runs = 3;
    struct trial trials[num_runs];
    unsigned size = 10000;
    unsigned growth = rand() % 9 + 2;
    for(unsigned i = 0; i < num_runs; ++i){
        size*=growth;
        int flag;
        if((flag = runTrial(size,&(trials[i])))){
            printf("ERROR! ");
            if(flag & FLAG_INSERTION){
                printf("INSERTION. ");
            }
            if(flag & FLAG_SELECTION){
                printf("SELECTION. ");
            }
            if(flag & FLAG_QUICK){
                printf("QUICK. ");
            }
            
        }
    }
    printTable(trials,num_runs);

    return 0;
}

// Generates a random array and copies it to three arrays
int generateArrays(unsigned size, struct data* d){

    // Allocates memory for the three arrays
    if((d->selection_sort_data.array = reallocarray(d->selection_sort_data.array, size, sizeof(int))) == NULL){
        perror("MALLOC A");
        exit(EXIT_FAILURE);
    }
    if((d->insertion_sort_data.array = reallocarray(d->insertion_sort_data.array, size, sizeof(int))) == NULL){
        perror("MALLOC B");
        exit(EXIT_FAILURE);
    }
    if((d->quick_sort_data.array = reallocarray(d->quick_sort_data.array, size, sizeof(int))) == NULL){
        perror("MALLOC C");
        exit(EXIT_FAILURE);
    }

    // Assigns values to the arrays
    for(unsigned i = 0; i < size; ++i){
        int val = rand() %10;
        d->selection_sort_data.array[i] = d->insertion_sort_data.array[i] = d->quick_sort_data.array[i] = val;
    }

    // Sets the size value for each array
    d->selection_sort_data.size = d->insertion_sort_data.size = d->quick_sort_data.size = size;

    return 0;
}

// Runs one trial of the three different sorting methods
int runTrial(unsigned elements, struct trial* trial_data){
    struct data array;
    initializeData(&array);
    generateArrays(elements,&array);
    trial_data->num_elements = elements;
    int flag = 0;
    if(executeSort(selectionSort,array.selection_sort_data,&(trial_data->selection_sort_time))){
        flag = flag | FLAG_SELECTION;
    }
    if(executeSort(InsertionSort,array.insertion_sort_data,&(trial_data->insertion_sort_time))){
        flag = flag | FLAG_INSERTION;
    }
    if(executeSort(QuickSortCaller,array.quick_sort_data,&(trial_data->quick_sort_time))){
        flag = flag | FLAG_QUICK;
    }
    return flag;
}

int executeSort(void (*f)(struct int_array), struct int_array arr, double* time){
    struct timeval start,stop;
    gettimeofday(&start, NULL);
    (*f)(arr);
    gettimeofday(&stop, NULL);
    saveTime(start,stop,time);
    return checkArray(arr);
}

void saveTime(struct timeval start, struct timeval stop, double* store){
    *store = (double)(stop.tv_sec - start.tv_sec) * 1000 + (double)(stop.tv_usec - start.tv_usec) / 1000;
}

// Initializes pointers to data arrays for trial data
void initializeData(struct data* d){
    d->selection_sort_data.array = NULL;
    d->insertion_sort_data.array = NULL;
    d->quick_sort_data.array = NULL;
}

int checkArray(struct int_array arr){
    for(unsigned i = 0; i < arr.size-1; ++i){
        if(arr.array[i] > arr.array[i+1]){
            return 1;
        }
    }
    return 0;
}

// Prints out the table head, each row, and the foot
void printTable(struct trial* trials, unsigned runs){
    printTableHead();
    for(unsigned i = 0; i < runs; ++i){
        printTableRow(trials[i]);
    }
    printTableFoot();
}


// Prints the head of the table
void printTableHead(){
    printf("//+-----------------------+-----------------+----------------+------------+\n");
    printf("//|    Number of Elements |  Selection Sort | Insertion sort |  Quicksort |\n");
    printf("//+-----------------------+-----------------+----------------+------------+\n");
}

// Prints a row of the table
void printTableRow(struct trial t){
    printf("//|%23u|%17f|%16f|%12f|\n",t.num_elements,t.selection_sort_time,t.insertion_sort_time,t.quick_sort_time);
}

// Prints the end of the table
void printTableFoot(){
    printf("//+-----------------------+-----------------+----------------+------------+\n");
}


// Selection Sort

// Prints out the elements of an integer array
void printSelectionSort(struct int_array arr){
    for(unsigned i = 0; i < arr.size; ++i){
        printf("%d ",arr.array[i]);
    }
    printf("\n");
}

// Performs selection sort on an array of integers
void selectionSort(struct int_array arr){
    for(unsigned i = 0; i <= (arr.size-2); ++i){
        unsigned min = i;
        // Finds the minimum element in the array
        for(unsigned j = i+1; j <= (arr.size-1); ++j){
            if(arr.array[j] < arr.array[min]){
                min = j;
            }
        }
        // Swaps the first element with the minimum one
        int temp = arr.array[i];
        arr.array[i] = arr.array[min];
        arr.array[min] = temp;
        // printSelectionSort(arr);
    }
}


// Quick Sort

// Prints out an array with a sub array focused in square brackets
void printQuickSort(struct int_array arr, struct slice slc){
    for(unsigned i = 0; i < arr.size; ++i){
        if(i == slc.l){
            printf("[%d ",arr.array[i]);
        } else if(i == slc.r){
            printf("%d]  ",arr.array[i]);
        } else {
            printf("%d ",arr.array[i]);
        }
    }
    printf("\n");
}

// Calls QuickSort(), passing the appropriate slice
void QuickSortCaller(struct int_array arr){
    struct slice slc;
    slc.l = 0;
    slc.r = arr.size - 1;
    QuickSort(arr,slc);
}

// Performs Quicksort on an array of integers
void QuickSort(struct int_array arr, struct slice slc){
    int l = slc.l, r = slc.r;
    if(l < r){
        int s  = hoarePartition(arr,slc);
        // printf("\n");
        // printSubArray(arr,slc);
        // printf("\n");
        slc.r = s-1;
        QuickSort(arr,slc);
        slc.r = r;
        slc.l = s+1;
        QuickSort(arr,slc);
        slc.l = l;
        // printQuickSort(arr,slc);
    }
}

// Swaps two elements of an array
void swap(int* A, int* B){
    int temp = *A;
    *A = *B;
    *B = temp;
}

// Performs Hoare partitioning for quicksort
int hoarePartition(struct int_array arr, struct slice slc){
    // printSubArray(arr,slc);
    int* array = arr.array;
    int p = array[slc.l];
    int i = slc.l;
    int j = slc.r+1;
    do{
        do {
            i++;
        }while(array[i] < p);
        do {
            j--;
        }while(array[j] > p);
        if(i < j){
            swap(&array[i],&array[j]);
        }
        // printSubArray(arr,slc);
    } while(i < j);
    // swap(&array[i],&array[j]);
    swap(&array[slc.l],&array[j]);
    // printSubArray(arr,slc);
    return j;
}


// Insertion Sort

// Prints out an array with a sub array focused in square brackets
void printInsertionSort(struct int_array arr, unsigned index){
    for(unsigned i = 0; i < arr.size; ++i){
        if(i == index+1){
            printf("| ");
        }
        printf("%d ",arr.array[i]);
        if(index == arr.size-1 && i == arr.size-1){
            printf("| ");
        }
    }
    printf("\n");
}

// Performs insertion sort.
void InsertionSort(struct int_array arr){
    // printInsertionSort(arr,0);
    for(unsigned i = 1; i <= arr.size-1; ++i){
        int v = arr.array[i];
        int j = i - 1;
        while(j >= 0 && arr.array[j] > v){
            arr.array[j+1] = arr.array[j];
            --j;
        }
        arr.array[j+1] = v;
        // printInsertionSort(arr,i);
    }

}
