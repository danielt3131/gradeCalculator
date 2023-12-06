#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
     */
    int *categorySizes = (int *) malloc(numberOfCategories * sizeof(int));
}
