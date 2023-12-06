#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int gradeStringSize = 50;

int main(int argc, char *argv[]) {
    FILE *gradeWeight = NULL;
    FILE *gradeBook = NULL;
    if (argc > 2){
        gradeWeight = fopen(argv[1], "r");
        gradeBook = fopen(argv[2], "r");
    } else {
        fprintf(stderr, "Unable to find files, pass in the file name as cli arguments in the order (grade weight) and grade book.\n");
        return 0;
    }
    int numberOfCategories;
    fscanf(gradeWeight, "%d", &numberOfCategories);
    /*
     * categorySizes stores the number of entries for each category
     * Allocating memory
     */
    int *categorySizes = (int *) malloc(numberOfCategories * sizeof(int));
    double *gradeWgt = (double *) malloc(numberOfCategories * sizeof(double));
    char **gradeWgtStr = (char *) malloc (numberOfCategories * sizeof(char *));
    char **gradeBookStr = (char **) malloc(numberOfCategories * sizeof(char *));
    double **grades = (double **) malloc(numberOfCategories * sizeof(double));
    for (int i = 0; i < numberOfCategories; i++){
        gradeWgtStr[i] = (char *) malloc(gradeStringSize * sizeof(char));
        gradeBookStr[i] = (char *) malloc(gradeStringSize * sizeof(char));
    }
    // Read in the grade weights with the associated string
    for (int i = 0; i < numberOfCategories; i++){
        fscanf(gradeWeight, "%50s:%lf%%\n", (gradeWgtStr + i), (gradeWgt + i));
    }
    int readCounter = 0;
    // Read until end of fil
    while (!feof(gradeBook)){
        fscanf(gradeBook, "%d", (categorySizes + readCounter));
        grades[readCounter] = malloc(categorySizes[readCounter] * sizeof(double));
        fscanf(gradeBook, "%50s", ())
        readCounter++;
    }
}
