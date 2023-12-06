#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int gradeStringSize = 50;

double average (double *grades, int numberOfGrades);
void removeLF (int numberOfStrings, char **string);

int main(int argc, char *argv[]) {
    FILE *gradeWeight = NULL;
    FILE *gradeBook = NULL;
    if (argc > 1){
        if(strcmp(argv[1], "-h") == 0){
            printf("gradeCalculator (Grade Weight) (Grade Book)\n");
            return 0;
        }
    }
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
    char **gradeWgtStr = (char **) malloc (numberOfCategories * sizeof(char *));
    char **gradeBookStr = (char **) malloc(numberOfCategories * sizeof(char *));
    double **grades = (double **) malloc(numberOfCategories * sizeof(double *));
    double *weightedGrade = (double *) malloc(numberOfCategories * sizeof(double));
    for (int i = 0; i < numberOfCategories; i++){
        gradeWgtStr[i] = (char *) malloc(gradeStringSize * sizeof(char));
        gradeBookStr[i] = (char *) malloc(gradeStringSize * sizeof(char));
    }
    char *readBuffer = (char *) malloc(gradeStringSize * sizeof(char));
    // Read in the grade weights with the associated string
    for (int i = 0; i < numberOfCategories; i++){
        fscanf(gradeWeight, "%50s %lf%%\n", readBuffer, &gradeWgt[i]); // &gradeWgt[i] -> (gradeWgt + i)
        strcpy(gradeWgtStr[i], readBuffer);
        gradeWgt[i] = gradeWgt[i] / 100; // Put back to decimal / undo percent
    }
    int readCounter = 0;
    // Read until end of the gradeBook file
    while (!feof(gradeBook)){
        fscanf(gradeBook, "%d\n", &categorySizes[readCounter]);
        grades[readCounter] = (double *) malloc(categorySizes[readCounter] * sizeof(double));
        fgets(gradeBookStr[readCounter], 50, gradeBook);
        for (int i = 0; i < categorySizes[readCounter]; i++){
            fscanf(gradeBook, "%lf%%\n", &grades[readCounter][i]);
            grades[readCounter][i] = grades[readCounter][i] / 100;  // Undo percentage
            //printf("%.2lf%% \n", grades[readCounter][i]);
        }
        readCounter++;
    }
    fclose(gradeBook);    // Close the grade book file
    fclose(gradeWeight);  // Close the grade weight file
    removeLF(numberOfCategories, gradeWgtStr);
    removeLF(numberOfCategories, gradeBookStr);
    /*
     * Grade calculation
     */
    for (int i = 0; i < numberOfCategories; i++){
        for (int j = 0; j < numberOfCategories; j++){
            if(strcmp(gradeBookStr[i], gradeWgtStr[j]) == 0){
                // Calculates the weighted grade for the category
                weightedGrade[i] = average(grades[i], categorySizes[i]) * gradeWgt[i];
            }
        }
    }
    double totalGrade = 0;
    for (int i = 0; i < numberOfCategories; i++){
        totalGrade += weightedGrade[i];
    }
    totalGrade *= 100; // For percentage format

    // Deallocating memory
    free(weightedGrade);
    free(gradeWgt);
    free(categorySizes);
    for (int i = 0; i < numberOfCategories; i++){
        free(gradeWgtStr[i]);
        free(gradeBookStr[i]);
        free(grades[i]);
    }
    free(gradeWgtStr);
    free(gradeBookStr);
    free(grades);

    printf("Your final grade will be %.4lf %%\n", totalGrade);
    printf("Do you want the calculated total grade saved?  If so press 1 and the file will be saved as grade.txt\n");
    printf("Otherwise press any key to quit the program\n");

    fgets(readBuffer, 50, stdin);
    if (strcmp(readBuffer, "1\n") == 0){
        FILE *result = fopen("grade.txt", "a");
        fprintf(result, "%.4lf%%\n", totalGrade);
        fclose(result);
        free(readBuffer);
        return 0;
    } else {
        free(readBuffer);
        return 0;
    }
}


double average (double *grades, int numberOfGrades){
    double average = 0;
    for (int i = 0; i < numberOfGrades; i++){
        average += grades[i];
    }
    average = average / numberOfGrades; // Calculates the average
    return average;
}
void removeLF (int numberOfStrings, char **string){
    for (int i = 0; i < numberOfStrings; i++){
        for (int j = 0; j <= strlen(string[i]); j++){
            if (string[i][j] == '\n'){
                string[i][j] = '\0'; // Insert null byte
            }
        }
    }
}