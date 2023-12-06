#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int gradeStringSize = 50;

double average (double *grades, int numberOfGrades);
void removeLF (int numberOfStrings, char **string);

int main(int argc, char *argv[]) {
    FILE *gradeWeight = fopen("gradeWeight.txt", "r");
    FILE *gradeBook = fopen("gradeBook.txt", "r");
/*    if (argc > 2){
        gradeWeight = fopen(argv[1], "r");
        gradeBook = fopen(argv[2], "r");
    } else {
        fprintf(stderr, "Unable to find files, pass in the file name as cli arguments in the order (grade weight) and grade book.\n");
        return 0;
    }
*/
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
        fscanf(gradeWeight, "%50s %lf%%\n", readBuffer, &gradeWgt[i]);
        strcpy(gradeWgtStr[i], readBuffer);
        gradeWgt[i] = gradeWgt[i] / 100; // Put back to decimal / undo percent
    }
    int readCounter = 0;
    // Read until end of the gradeBook file
    while (!feof(gradeBook)){
        fgets(readBuffer, 50, gradeBook);
        categorySizes[readCounter] = atoi(readBuffer);
        grades[readCounter] = (double *) malloc(categorySizes[readCounter] * sizeof(double));
        fgets(readBuffer, 50, gradeBook);
        printf("%s\n", readBuffer);
        strcpy(gradeBookStr[readCounter], readBuffer);
        for (int i = 0; i < categorySizes[readCounter]; i++){
            fscanf(gradeBook, "%lf%%\n", &grades[readCounter][i]);
            grades[readCounter][i] = grades[readCounter][i] / 100;
            printf("%.2lf%% \n", grades[readCounter][i]);  // Undo percentage
        }
        readCounter++;
    }
    fclose(gradeBook);
    fclose(gradeWeight);
    removeLF(numberOfCategories, gradeWgtStr);
    removeLF(numberOfCategories, gradeBookStr);
    /*
     * Grade calculation
     */
    for (int i = 0; i < numberOfCategories; i++){
        for (int j = 0; j < numberOfCategories; j++){
            printf("%s | %s \n", gradeBookStr[i], gradeWgtStr[j]);
            if(strcmp(gradeBookStr[i], gradeWgtStr[j]) == 0){
                weightedGrade[i] = average(grades[i], categorySizes[i]) * gradeWgt[i];
                printf("Hello\n");// Calculates the weighted grade for the category
            }
        }
    }
    double totalGrade = 0;
    for (int i = 0; i < numberOfCategories; i++){
        totalGrade += weightedGrade[i];
    }
    totalGrade *= 100;
    printf("%.4lf %%", totalGrade);
    // Deallocating memory
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