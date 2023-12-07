/*
 * A Grade Book calculator
 * Copyright (c) 2023 Daniel J. Thompson.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 or later.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Grade{
    char gradeCategory[50];
    double *grades;
    double gradeWeight;
    unsigned long numberOfGrades;
}Grade;

const int gradeStringSize = 50;

double average (const double *grades, unsigned long numberOfGrades);
void removeLF (char *string);

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
        // Prints to the standard error (still in the cli) and terminates the program
        fprintf(stderr, "Unable to find files, pass in the file name as cli arguments in the order (grade weight) and grade book.\n");
        return 0;
    }
    int numberOfCategories;
    // Reading in the number of categories -> example Homework, Exams, Attendance will be 3
    fscanf(gradeWeight, "%d", &numberOfCategories);
    /*
     * Allocating memory for the arrays, to be dynamic
     */
    // Stores the number of grades per category
    Grade *grade = (Grade *) malloc(numberOfCategories * sizeof(Grade));
//    int *categorySizes = (int *) malloc(numberOfCategories * sizeof(int));
    double *gradeWgt = (double *) malloc(numberOfCategories * sizeof(double));
    char **gradeWgtStr = (char **) malloc (numberOfCategories * sizeof(char *));
//    char **gradeBookStr = (char **) malloc(numberOfCategories * sizeof(char *));
//    double **grades = (double **) malloc(numberOfCategories * sizeof(double *));
    double *weightedGrade = (double *) malloc(numberOfCategories * sizeof(double));
    // 2D array allocation
    for (int i = 0; i < numberOfCategories; i++){
        gradeWgtStr[i] = (char *) malloc(gradeStringSize * sizeof(char));
//        gradeBookStr[i] = (char *) malloc(gradeStringSize * sizeof(char));
    }
    char *readBuffer = (char *) malloc(gradeStringSize * sizeof(char));
    // Read in the grade weights with the associated string
    for (int i = 0; i < numberOfCategories; i++){
        //
        fscanf(gradeWeight, "%50s %lf%%\n", readBuffer, &gradeWgt[i]); // &gradeWgt[i] -> (gradeWgt + i)
        strcpy(gradeWgtStr[i], readBuffer);// Copies the string in the readBuffer to gradeWgtStr at i
        removeLF(gradeWgtStr[i]);
        gradeWgt[i] = gradeWgt[i] / 100; // Put back to decimal / undo percent
    }
    fclose(gradeWeight);  // Close the grade weight file
    int readCounter = 0;
    /*
    * Removes \n from strings to ensure strcmp works correctly
    */

    // Read until end of the gradeBook file
    while (!feof(gradeBook)){
        fscanf(gradeBook, "%lu\n", &grade[readCounter].numberOfGrades);
        // Jagged array for the grade entries
        grade[readCounter].grades = (double *) malloc(grade[readCounter].numberOfGrades * sizeof(double));
        // Reading in the string for the grade category -> example Homework
        fgets(readBuffer, 50, gradeBook);
        removeLF(readBuffer);
        // Makes sure that the grade weight entries will goto the correct struct
        for (int i = 0; i < numberOfCategories; i++){
            if(strcmp(readBuffer, gradeWgtStr[i]) == 0){
                strcpy(grade[readCounter].gradeCategory, readBuffer);
                grade[readCounter].gradeWeight = gradeWgt[i];
            }
        }
        for (int i = 0; i < grade[readCounter].numberOfGrades; i++){
            fscanf(gradeBook, "%lf%%\n", &grade[readCounter].grades[i]);
            grade[readCounter].grades[i] = grade[readCounter].grades[i] / 100;  // Undo percentage |-> percent % = n * 100
            //printf("%.2lf%% \n", grade[readCounter].grades[i]);  // <- Debug line
        }
        readCounter++;
    }
    fclose(gradeBook);    // Close the grade book file
    /*
     * Grade calculation
     */
    for (int i = 0; i < numberOfCategories; i++){
        weightedGrade[i] = average(grade[i].grades, grade[i].numberOfGrades) * grade[i].gradeWeight;
    }
    double totalGrade = 0;
    for (int i = 0; i < numberOfCategories; i++){
        totalGrade += weightedGrade[i];
    }

    totalGrade *= 100; // For percentage format
    // Deallocating memory |-> giving ownership back to the operating system to ensure no memory leaks.
    free(weightedGrade);
    free(gradeWgt);
    for (int i = 0; i < numberOfCategories; i++){
        free(gradeWgtStr[i]);
        free(grade[i].grades);
    }
    free(gradeWgtStr);
    free(grade);

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

// const -> the values don't change
double average (const double *grades, unsigned long numberOfGrades){
    double average = 0;
    for (int i = 0; i < numberOfGrades; i++){
        average += grades[i];  // Sum up the array
    }
    average = average / numberOfGrades; // Calculates the average
    return average;
}
void removeLF (char *string){
    for (int j = 0; j <= strlen(string); j++){
        // If the character at [i][j] is equal to \n (ASCII value of 10)
        if (string[j] == '\n'){
            string[j] = '\0'; // Insert null byte (ASCII value of 0) to terminate the string
        }
    }
}