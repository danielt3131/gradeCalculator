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
#include <stdbool.h>

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
        fprintf(stderr, "Unable to find files, pass in the file name as cli arguments in the order grade weight and grade book.\n");
        fprintf(stderr, "For more info about the cli arguments pass -h to see the help page\n");
        fprintf(stderr, "Now exiting\n");
        return 0;
    }
    int numberOfCategories;
    // Reading in the number of categories -> example Homework, Exams, Attendance will be 3
    fscanf(gradeWeight, "%d", &numberOfCategories);
    Grade *grade = (Grade *) malloc(numberOfCategories * sizeof(Grade));
    /*
     * Allocating memory for the arrays, to be dynamic
     */
    double *gradeWgt = (double *) malloc(numberOfCategories * sizeof(double));
    char **gradeWgtStr = (char **) malloc (numberOfCategories * sizeof(char *));
    double *weightedGrade = (double *) malloc(numberOfCategories * sizeof(double));
    // 2D array allocation
    for (int i = 0; i < numberOfCategories; i++){
        gradeWgtStr[i] = (char *) malloc(gradeStringSize * sizeof(char));
    }
    char *readBuffer = (char *) malloc(gradeStringSize * sizeof(char));
    // Read in the grade weights with the associated string
    for (int i = 0; i < numberOfCategories; i++){
        //
        fscanf(gradeWeight, "%50s %lf%%\n", readBuffer, &gradeWgt[i]); // &gradeWgt[i] -> (gradeWgt + i)
        strcpy(gradeWgtStr[i], readBuffer);// Copies the string in the readBuffer to gradeWgtStr at i
        removeLF(gradeWgtStr[i]); // Removes \n Linefeed (ASCII value 10) for strcmp
        gradeWgt[i] = gradeWgt[i] / 100; // Put back to decimal / undo percent
    }
    fclose(gradeWeight);  // Close the Grade Weight file as it's no longer needed
    int readCounter = 0;
    char modeSelectorChar;
    bool percentFormat;    // True = percent format, false = fractional format
    fscanf(gradeBook, "%50s %c\n", readBuffer, &modeSelectorChar);
    if (modeSelectorChar == '%' && strcmp(readBuffer, "Mode:") == 0){
        percentFormat = true;
    } else if (modeSelectorChar == '/' && strcmp(readBuffer, "Mode:") == 0){
        percentFormat = false;
    } else {
        fseek(gradeBook, 0, SEEK_SET); // Goto the start of the Grade Book file for the upcoming fscanf
        percentFormat = true;  // Default format is percentage.
    }
    // Read until end of the Grade Book file
    while (!feof(gradeBook)){
        // Stores the number of grades into the struct at index readCounter.
        fscanf(gradeBook, "%lu\n", &grade[readCounter].numberOfGrades);
        // Jagged array for the grade entries
        grade[readCounter].grades = (double *) malloc(grade[readCounter].numberOfGrades * sizeof(double));
        // Reading in the string for the grade category -> example Homework
        fgets(readBuffer, 50, gradeBook);
        removeLF(readBuffer); // Removes \n Linefeed (ASCII value 10) for strcmp
        /*
         * Makes sures that the grade weight will be set to the correct struct
         * By comparing the grade category string from the grade book to all grade categories read in from gradeWeight
         * If successful then the gradeWeight variable in the struct is set to the correct value
         */
        for (int i = 0; i < numberOfCategories; i++){
            if(strcmp(readBuffer, gradeWgtStr[i]) == 0){
                strcpy(grade[readCounter].gradeCategory, readBuffer);
                grade[readCounter].gradeWeight = gradeWgt[i];
            }
        }
        for (int i = 0; i < grade[readCounter].numberOfGrades; i++){
            if (percentFormat == true){
                fscanf(gradeBook, "%lf%%\n", &grade[readCounter].grades[i]);
                // Undo percentage |-> percent % = n * 100
                grade[readCounter].grades[i] = grade[readCounter].grades[i] / 100;
            } else {
                double numerator = 0;
                // Store the numerator to the variable numerator and the denominator to grade[readCounter].grades[i]
                fscanf(gradeBook,"%lf/%lf\n", &numerator, &grade[readCounter].grades[i]);
                /*
                 * Get the decimal representation by dividing the numerator by the denominator.
                 * Store the result back into grade[readCounter].grades[i].
                 */
                grade[readCounter].grades[i] = numerator / grade[readCounter].grades[i];
            }
        }
        readCounter++;
    }
    fclose(gradeBook);    // Close the grade book file
    /*
     * Grade calculation
     */
    for (int i = 0; i < numberOfCategories; i++) {
        weightedGrade[i] = average(grade[i].grades, grade[i].numberOfGrades) * grade[i].gradeWeight;
        // weightedGrade[i] / grade[i] removes the grade weight & multiplying by 100 converts the grade to percent format
        printf("Your average %s grade is %.4lf %%\n", grade[i].gradeCategory, (weightedGrade[i] / grade[i].gradeWeight) * 100);
    }
    double totalGrade = 0;
    for (int i = 0; i < numberOfCategories; i++){
        totalGrade += weightedGrade[i];  // Sum up all of the weightedGrades as totalGrade
    }
    totalGrade *= 100; // To covert to percentage format

    // Deallocating memory |-> giving ownership back to the operating system to ensure no memory leaks.
    free(weightedGrade);
    free(gradeWgt);
    for (int i = 0; i < numberOfCategories; i++){
        free(gradeWgtStr[i]);
        free(grade[i].grades);
    }
    free(gradeWgtStr);
    free(grade);
    printf("\n");
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